# Notes

Explanations worth keeping, from conversations alongside the code. Not conventions
(those are in `CLAUDE.md`) and not problem descriptions (those are in `README.md`).
Entries are kept as written rather than edited down.

## CLAUDE.md vs README.md

The core split is audience and purpose. A README explains what a project *is* to a person deciding whether to use or understand it. A CLAUDE.md tells an agent how to *change* it correctly. One is description, the other is policy.

That difference drives most of the rest. A README is read selectively — someone skims for the install command, or reads one section closely — so it can afford to be long and interesting. Your "Why it's interesting" paragraphs are the best example: pure value for a human reader, zero operational content. A CLAUDE.md gets loaded in full at the start of every session and costs context each time, so it earns its keep only by being terse and prescriptive. It should contain the things an agent would otherwise get wrong by default, which is mostly invariants ("language order is always C++, Python, Scala, Rust"), workflow ("update the README in the same change"), and hard-won gotchas that cost a compile cycle to rediscover.

The other asymmetry is tense. A README documents the current state. A CLAUDE.md constrains future states — it's written for work that hasn't happened yet.

What they share is the orientation layer: build and run commands, directory structure, tech stack, naming conventions. This is where people get into trouble, because duplicating that content means two copies drifting apart. The usual resolution is that CLAUDE.md points rather than restates — "see README.md for the problem descriptions" — and only duplicates what's short enough that drift is obvious, like the four run commands.

Failure modes differ too, and that's worth keeping in mind. A stale README confuses one person, who then asks a question. A stale CLAUDE.md gets silently applied across a dozen files before anyone notices. That argues for keeping CLAUDE.md small: every line is a rule you're committing to maintain.

## Decoupling index and size types in matrix and graph objects

An index and a position are different things, and the four languages differ sharply in whether they let you say so.

An **index** names an entity: a row, a column, a vertex, a supernode. A **position** locates something inside one particular flat array and means nothing outside it. In a CSR matrix, `colIdx` holds indices and `rowPtr` holds positions, and both are subscripted by a position.

**Why the index wants to be 32-bit.** An index array is as long as the matrix has nonzeros, so its element width is a first-order memory cost. Capping an index at 2^31 is cheap, since a matrix dimension that large is already enormous, and it halves the size of the largest arrays in the program. A signed type also gives a free `NIL = -1` sentinel, which graph and elimination-tree code uses constantly. C++ spells this `std::int32_t`, Rust `i32`.

**Why the position wants to be 64-bit unsigned.** A position measures rather than names. It is never negative and never `NIL`, and its range needs to cover the total number of stored entries, not the dimension. That is the whole point: with 32-bit indices the dimension caps at 2^31, but nnz is bounded by n^2, so positions need roughly 2^62 of range to cover what those indices can address. `std::size_t` and `usize` supply it.

**So the decoupling buys about 31 bits of range for free.** Coupling the two, using one type for both, throws away everything the index cap was supposed to make affordable and gains nothing, since the `NIL` sentinel was already available on the index side.

### Where each language lands

- **C++ and Rust: full support.** `std::int32_t` / `std::size_t` and `i32` / `usize`. Both express the distinction in the type system, and the compiler flags the crossings. The cost differs: C++'s usual arithmetic conversions handle a subscript silently, so `x.val[a.colIdx[rp]]` needs no cast, while Rust writes out every crossing with `as usize`. Different friction, same expressive power.
- **Python: no problem at all, at the other extreme.** Integers are arbitrary-precision, so no cap exists in either role. The distinction survives only as naming, and nothing is lost by that, because nothing was constrained to begin with.
- **Scala: the difficult case.** `Int`, `Long`, `Short`, `Byte`, all signed, no unsigned type at all. The index half is fine, since `Int` is exactly a 32-bit signed integer. The position half has nowhere to go, and that is the reverse of what one might guess: the problem is not that Scala lacks a type for indices, it is that it lacks one for positions.

### Why Scala's limit is structural rather than stylistic

`Array[Long]` would widen a pointer array, at the cost of `.toInt` on every subscript. But it would not help, because the JVM indexes arrays with `int`: no array can hold more than `Int.MaxValue` elements, whatever its element type. Widening the pointer does not widen the thing it points into.

So a flat value buffer simply cannot hold 2^31 nonzeros on the JVM. The fix is not a wider declaration but a different layout, chunked or paged storage, one array per block rather than one array for everything.

This reframes a design choice that looks like a performance question elsewhere. In C++ and Rust, flat storage versus chunked storage is about locality and allocation strategy, and either is viable at any size. On the JVM, above a certain size, chunking stops being a choice.

### Why this bites hardest in the sparse case

For a matrix `A`, nnz is known when the object is built, so a cap is one check in the constructor and nothing more.

For a factor `L`, nnz is discovered. Symbolic factorization computes it from the fill pattern, and with dynamic pivoting it is not final until the numeric phase has run, since a delayed column expands a front at run time. Holding a factor under a cap therefore means checking at every site that extends the structure, correctly, forever. With a 64-bit position type there is no check to write, because nothing reachable can overflow.

The dense case is milder but the same shape: `int32` indices can address an `m x n` matrix whose entry count exceeds what a 32-bit position can reach, so even without any sparsity the two types are not interchangeable.
