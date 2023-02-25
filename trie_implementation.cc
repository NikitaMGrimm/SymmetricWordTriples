template<typename T>
class TrieNode {
public:
    std::unordered_map<T, TrieNode<T>*> children;
    bool is_word = false;

    ~TrieNode() {
        for (auto& [c, child] : children) {
            delete child;
        }
    }
};

template<typename T>
class Trie {
public:
    Trie() : root(new TrieNode<T>()) {}

    ~Trie() {
        delete root;
    }

    void Insert(const std::vector<T>& word) {
        TrieNode<T>* node = root;
        for (T letter : word) {
            if (!node->children.count(letter)) {
                node->children[letter] = new TrieNode<T>();
            }
            node = node->children[letter];
        }
        node->is_word = true;
    }

    bool Search(const std::vector<T>& word) const {
        const TrieNode<T>* node = FindNode(word);
        return node && node->is_word;
    }

    bool StartsWith(const std::vector<T>& prefix) const {
        return FindNode(prefix) != nullptr;
    }

    std::vector<std::vector<T>> GetWordsWithPrefix(const std::vector<T>& prefix) const {
        std::vector<std::vector<T>> words;
        const TrieNode<T>* node = FindNode(prefix);
        if (node) {
            CollectWords(prefix, node, words);
        }
        return words;
    }

    std::vector<std::vector<T>> GetAllWords() const {
        std::vector<std::vector<T>> words;
        CollectWords({}, root, words);
        return words;
    }

    std::vector<std::vector<T>> GetAllWordsOfSize(int size) const {
        std::vector<std::vector<T>> words;
        CollectWordsOfSize({}, root, size, words);
        return words;
    }

    std::vector<std::vector<T>> MatchPrefix(const T& prefix) const {
        std::vector<std::vector<T>> words;
        const TrieNode<T>* node = root;
        for (T letter : prefix) {
            if (!node->children.count(letter)) {
                return words;
            }
            node = node->children.at(letter);
        }
        CollectWords(prefix, node, words);
        return words;
    }

private:
    TrieNode<T>* root;

    const TrieNode<T>* FindNode(const std::vector<T>& word) const {
        const TrieNode<T>* node = root;
        for (T letter : word) {
            if (!node->children.count(letter)) {
                return nullptr;
            }
            node = node->children.at(letter);
        }
        return node;
    }

    void CollectWords(const std::vector<T>& prefix, const TrieNode<T>* node, std::vector<std::vector<T>>& words) const {
        if (node->is_word) {
            words.push_back(prefix);
        }
        for (auto it = node->children.begin(); it != node->children.end(); ++it) {
            const T& c = it->first;
            const TrieNode<T>* child = it->second;
            CollectWords(prefix + std::vector{c}, child, words);
        }
    }

    void CollectWordsOfSize(const std::vector<T>& prefix, const TrieNode<T>* node, int size, std::vector<std::vector<T>>& words) const {
        if (size == 0) {
            if (node->is_word) {
                words.push_back(prefix);
            }
        } else {
            for (auto [c, child] : node->children) {
                std::vector<T> new_prefix = prefix;
                new_prefix.push_back(c);
                CollectWordsOfSize(new_prefix, child, size - 1, words);
            }
        }
    }
};