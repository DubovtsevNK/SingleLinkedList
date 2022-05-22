#pragma once
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>
#include <vector>

template <typename Type>
class SingleLinkedList {

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {

        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) : node_(node) {
        }

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept {
            if(*this != other)
            {
                this->node_ = other.node_;
            }
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {

            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            auto tmp = node_->next_node;
            node_ = tmp;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            BasicIterator tmp (*this);
            auto next = node_->next_node;
            node_ = next;
            return tmp;

        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }


        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };
private:
    Node head_;
    size_t size_;

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;

    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return  ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return  ConstIterator{nullptr};
    }


    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return  ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return  ConstIterator{nullptr};
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }


public:

    SingleLinkedList () : head_(), size_(0){};

    SingleLinkedList(std::initializer_list<Type> values) : size_(0) {
        for(auto a = rbegin(values); a != rend(values); a++)
        {
            PushFront(*a);
        }
    }

    SingleLinkedList(const SingleLinkedList& other): size_(0) {

        if(!other.IsEmpty())
        {
            auto begin = other.begin();
            Node** node_ptr = &head_.next_node;
            while (begin != other.end())
            {
                *node_ptr = new Node(*begin, nullptr);
                node_ptr = &((*node_ptr)->next_node);
                ++begin;
            }
        }
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if(this != &rhs)
        {
            SingleLinkedList<Type> tmp (rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(SingleLinkedList& other) noexcept {
        if(this != &other)
        {
            using std::swap;
            swap(head_.next_node, other.head_.next_node);
            swap(size_, other.size_);
        }
    }


    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        size_++;
    }

    void Clear() noexcept {
        while(head_.next_node)
        {
            PopFront();
        }

    }
    void PopFront() noexcept
    {
        if(head_.next_node)
        {
            Node *next = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = next;
            size_--;
        }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr);
        Node *tmp = pos.node_->next_node;
        pos.node_->next_node = tmp->next_node;
        delete tmp;
        return Iterator{pos.node_->next_node};
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node * tmp = new Node(value, pos.node_->next_node);
        pos.node_->next_node = tmp;
        size_++;
        return Iterator{pos.node_->next_node};
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return head_.next_node == nullptr;
    }
    ~SingleLinkedList()
    {
        Clear();
    }


};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(),lhs.end(),rhs.begin());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs == rhs) or (lhs < rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (lhs == rhs) or (lhs > rhs);
}
