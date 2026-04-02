#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <array>

/**
 * Implementation A: Hash map trie (flexible alphabet).
 *
 * Each node stores children in an unordered_map<char, unique_ptr<Node>>.
 * This supports any character set without wasting memory on unused slots.
 * Lookup and insertion are O(L) where L is the word length, with O(1)
 * amortized per character via the hash map.
 *
 * unique_ptr handles cleanup automatically — when a node is destroyed,
 * all its children are recursively freed.
 */
class HashMapTrie {
private:
    struct Node {
        std::unordered_map<char, std::unique_ptr<Node>> children;
        bool is_end = false;
    };

    Node root;

    Node* traverse(const std::string& s) {
        Node* current = &root;
        for (char c : s) {
            auto it = current->children.find(c);
            if (it == current->children.end()) return nullptr;
            current = it->second.get();
        }
        return current;
    }

public:
    void insert(const std::string& word) {
        Node* current = &root;
        for (char c : word) {
            auto& child = current->children[c];
            if (!child) {
                child = std::make_unique<Node>();
            }
            current = child.get();
        }
        current->is_end = true;
    }

    bool search(const std::string& word) {
        Node* node = traverse(word);
        return node != nullptr && node->is_end;
    }

    bool starts_with(const std::string& prefix) {
        return traverse(prefix) != nullptr;
    }
};

/**
 * Implementation B: Fixed-array trie (lowercase ASCII only).
 *
 * Each node stores children in a fixed array of 26 unique_ptrs, one per
 * letter a-z. Lookup is a direct index: children[c - 'a']. This trades
 * memory (26 pointers per node regardless of usage) for speed (no hashing,
 * no collision handling, perfect cache-line alignment per level).
 *
 * For sparse tries with few children per node, the hash map version
 * uses less memory. For dense tries (e.g. dictionaries), the array
 * version is faster due to direct indexing.
 */
class ArrayTrie {
private:
    struct Node {
        std::array<std::unique_ptr<Node>, 26> children{};
        bool is_end = false;
    };

    Node root;

    Node* traverse(const std::string& s) {
        Node* current = &root;
        for (char c : s) {
            int idx = c - 'a';
            if (!current->children[idx]) return nullptr;
            current = current->children[idx].get();
        }
        return current;
    }

public:
    void insert(const std::string& word) {
        Node* current = &root;
        for (char c : word) {
            int idx = c - 'a';
            if (!current->children[idx]) {
                current->children[idx] = std::make_unique<Node>();
            }
            current = current->children[idx].get();
        }
        current->is_end = true;
    }

    bool search(const std::string& word) {
        Node* node = traverse(word);
        return node != nullptr && node->is_end;
    }

    bool starts_with(const std::string& prefix) {
        return traverse(prefix) != nullptr;
    }
};
