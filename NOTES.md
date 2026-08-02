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
