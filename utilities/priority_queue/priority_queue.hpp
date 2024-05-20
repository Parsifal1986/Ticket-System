#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include "../exceptions/sjtu_exceptions.hpp"
#include <cstddef>
#include <functional>

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template <typename T, class Compare = std::less<T>> class priority_queue {
public:
  /**
   * TODO constructors
   */
  priority_queue() {
    head = new node();
    head->sibling = nullptr;
    min_node = head->sibling;
  }
  priority_queue(const priority_queue &other) {
    head = new node();
    head->sibling = Copy(other.head->sibling, nullptr);
    heap_size = other.heap_size;
    node *p = min_node = head->sibling;
    while (p != nullptr) {
      if (comp(*((T *)min_node->data), *((T *)p->data))) {
        min_node = p;
      }
      p = p->sibling;
    }
  }
  /**
   * TODO deconstructor
   */
  ~priority_queue() { Delete(head); }
  /**
   * TODO Assignment operator
   */
  priority_queue &operator=(const priority_queue &other) {
    if (this == &other) {
      return *this;
    }
    Delete(head);
    head = new node();
    head->sibling = Copy(other.head->sibling, head);
    heap_size = other.heap_size;
    node *p = min_node = head->sibling;
    while (p != nullptr) {
      if (comp(*((T *)min_node->data), *((T *)p->data))) {
        min_node = p;
      }
      p = p->sibling;
    }
    return *this;
  }
  /**
   * get the top of the queue.
   * @return a reference of the top element.
   * throw container_is_empty if empty() returns true;
   */
  const T &top() {
    if (empty()) {
      throw container_is_empty();
    }
    return *((T *)min_node->data);
  }
  /**
   * TODO
   * push new element to the priority queue.
   */
  void push(const T &e) {
    if (min_node != nullptr) {
      comp(*((T *)min_node->data), e);
    }
    heap_size++;
    node *tmp = new node(e);
    tmp->sibling = head->sibling;
    head->sibling = update(tmp);
    if (min_node == nullptr || comp(*((T *)min_node->data), e)) {
      min_node = head->sibling;
    }
    return;
  }
  /**
   * TODO
   * delete the top element.
   * throw container_is_empty if empty() returns true;
   */
  void pop() {
    if (empty()) {
      throw container_is_empty();
    }
    node *p = head;
    while (p->sibling != min_node) {
      p = p->sibling;
    }
    p->sibling = min_node->sibling;
    priority_queue<T, Compare> *tmp = init(min_node->left_child);
    merge(*tmp);
    delete min_node;
    heap_size--;
    p = min_node = head->sibling;
    while (p != nullptr) {
      if (comp(*((T *)min_node->data), *((T *)p->data))) {
        min_node = p;
      }
      p = p->sibling;
    }
    tmp->head = new node();
    delete tmp;
  }
  /**
   * return the number of the elements.
   */
  size_t size() const { return heap_size; }
  /**
   * check if the container has at least an element.
   * @return true if it is empty, false if it has at least an element.
   */
  bool empty() const { return !heap_size; }
  /**
   * merge two priority_queues with at least O(logn) complexity.
   * clear the other priority_queue.
   */
  void merge(priority_queue &other) {
    node *from_pos = other.head->sibling, *to_pos;
    while (from_pos != nullptr) {
      to_pos = head;
      while (to_pos->sibling != nullptr &&
             to_pos->sibling->degree < from_pos->degree) {
        to_pos = to_pos->sibling;
      }
      node *tmp = from_pos->sibling;
      from_pos->sibling = to_pos->sibling;
      to_pos->sibling = from_pos;
      to_pos->sibling = update(to_pos->sibling);
      from_pos = tmp;
    }
    other.head->sibling = from_pos;
    Delete(other.head);
    other.heap_size = 0;
    return;
  }

private:
  Compare comp;
  struct node {
    node() {
      data = nullptr;
      father = left_child = sibling = nullptr;
      degree = 1;
    }

    node(T data_, node *father_ = nullptr, node *left_child_ = nullptr,
         node *sibling_ = nullptr, int degree_ = 1)
        : father(father_), left_child(left_child_), sibling(sibling_),
          degree(degree_) {
      data = operator new(sizeof(T));
      new (data) T(data_);
    }

    node(const node &other)
        : father(other.father), left_child(other.left_child),
          sibling(other.sibling), degree(other.degree) {

      if (other.data == nullptr) {
        data = nullptr;
        return;
      }
      data = operator new(sizeof(T));
      new (data) T(*((T *)other.data));
    }

    ~node() {
      if (data == nullptr) {
        return;
      }
      ((T *)data)->~T();
      operator delete(data);
    }

    void operator=(const node &other) {
      if (other == *this) {
        return;
      }
      new (data) T((T *)other->data);
      father = other.father;
      left_child = other.left_child;
      sibling = other.sibling;
      degree = other.degree;
    }

    void *data;
    node *father, *left_child, *sibling;
    int degree;
  };
  node *head, *min_node;
  int heap_size = 0;

private:
  node *Copy(node *target, node *pos_father) {
    if (target == nullptr) {
      return nullptr;
    }
    node *tmp = new node(*target);
    tmp->father = pos_father;
    tmp->left_child = Copy(target->left_child, tmp);
    tmp->sibling = Copy(target->sibling, pos_father);
    return tmp;
  }

  void Delete(node *pos) {
    if (pos->left_child != nullptr) {
      Delete(pos->left_child);
    }
    if (pos->sibling != nullptr) {
      Delete(pos->sibling);
    }
    delete pos;
  }

  node *Union(node *root1, node *root2) {
    if (comp((*((T *)root2->data)), (*((T *)root1->data)))) {
      root2->father = root1;
      root2->sibling = root1->left_child;
      root1->left_child = root2;
      root1->degree = root1->degree + root2->degree;
      return root1;
    } else {
      root1->father = root2;
      root1->sibling = root2->left_child;
      root2->left_child = root1;
      root2->degree = root1->degree + root2->degree;
      return root2;
    }
  }

  node *update(node *pos) {
    if (pos->sibling == nullptr) {
      return pos;
    }
    if (pos->sibling->degree == pos->degree) {
      node *tmp = pos->sibling->sibling;
      pos = Union(pos, pos->sibling);
      pos->sibling = tmp;
      pos = update(pos);
    }
    return pos;
  }

  priority_queue<T, Compare> *init(node *new_head) {
    priority_queue<T, Compare> *new_queue = new priority_queue<T, Compare>;
    new_queue->head->sibling = new_head;
    return new_queue;
  }
};

} // namespace sjtu

#endif
