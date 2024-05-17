/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <cstddef>
#include <functional>
#include <new>
#include "utility.hpp"
#include "../exceptions/sjtu_exceptions.hpp"

#define COLOR(p) (p == nullptr ? BLACK : p->color)
#define OUTWARD(p) (p->father->left_child == p ? p->left_child : p->right_child)
#define INWARD(p) (p->father->left_child == p ? p->right_child : p->left_child)

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;

private:
	enum Color {BLACK, RED};

  struct node {
    value_type data;
    node *left_child, *right_child, *father;
    Color color;

		node() : data(*((value_type*)(operator new (sizeof(value_type))))) {
			color = BLACK;
		}

    node(value_type value)
        : data(value), left_child(nullptr), right_child(nullptr),
          father(nullptr), color(RED) {}

    node(value_type value, node *parent, node *left, node *right)
        : data(value), father(parent), left_child(left), right_child(right),
          color(RED) {}

    // node & operator=(const node &other) {
    // 	if (this == &other) {
    // 		return *this;
    // 	}
    // 	data.first = other.data.first;
		// 	data.second = other.data.second;
		// 	color = other.color;
		// 	return *this;
    // }

    ~node() = default;
  };
public:
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
		friend class const_iterator;
		friend class map;
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		node *pointer;
		const map<Key, T, Compare> *belong;

		node* back() {
			node *p = belong->root;
			if (p == nullptr) {
				return nullptr;
			} else {
				while (p->right_child != nullptr) {
					p = p->right_child;
				}
			}
			return p;
		}

	public:
		iterator() : pointer(nullptr), belong(nullptr) {

		}
		iterator(const const_iterator &other) : pointer(other.pointer), belong(other.belong) {
			
		}
		iterator(const iterator &other) : pointer(other.pointer), belong(other.belong) {
		
		}
		iterator(node *p, const map<Key, T, Compare> *belong) : pointer(p), belong(belong) {
			
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			if (pointer == belong->watcher) {
				throw invalid_iterator();
			}
			iterator tmp(*this);
			pointer = find_next(pointer);
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if (pointer == belong->watcher) {
				throw invalid_iterator();
			}
			pointer = find_next(pointer);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			if (pointer == belong->watcher) {
				throw invalid_iterator();
			}
			iterator tmp(*this);
				if (pointer == nullptr) {
					pointer = back();
				} else {
					pointer = find_prev(pointer);
				}
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if (pointer == belong->watcher) {
				throw invalid_iterator();
			}
			if (pointer == nullptr) {
				pointer = back();
			} else {
				pointer = find_prev(pointer);
			}
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
			if (pointer == nullptr) {
				throw invalid_iterator();
			}
			return (*pointer).data;
		}
		bool operator==(const iterator &rhs) const {
			if (belong == rhs.belong) {
				if (pointer == belong->watcher && rhs.pointer == nullptr) {
					return true;
				} else if (pointer == nullptr && rhs.pointer == belong->watcher) {
					return true;
				}
			} else {
				return false;
			}
			return rhs.pointer == this->pointer;
		}
		bool operator==(const const_iterator &rhs) const {
			if (belong == rhs.belong) {
				if (pointer == belong->watcher && rhs.pointer == nullptr) {
					return true;
				} else if (pointer == nullptr && rhs.pointer == belong->watcher) {
					return true;
				}
			} else {
				return false;
			}
			return rhs.pointer == this->pointer;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return !(*this == rhs);
		}
		bool operator!=(const const_iterator &rhs) const {
			return !(*this == rhs);
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const {
			if (pointer == nullptr) {
				throw invalid_iterator();
			}
			return &(pointer->data);
		}
	};
	class const_iterator {
		friend class iterator;
		friend class map;
		private:
			node *pointer;
			const map<Key, T, Compare> *belong;

		node* back() {
			node *p = belong->root;
			if (p == nullptr) {
				return nullptr;
			} else {
				while (p->right_child != nullptr) {
					p = p->right_child;
				}
			}
			return p;
		}
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.

		public:
			const_iterator() : pointer(nullptr), belong(nullptr) {
			
			}

			const_iterator(const const_iterator &other) : pointer(other.pointer), belong(other.belong) {
			
			}
			const_iterator(const iterator &other) : pointer(other.pointer), belong(other.belong) {
			
			}
			const_iterator(node *p, const map<Key, T, Compare> *belong) : pointer(p), belong(belong) {

			}

			const value_type & operator*() const {
				if (pointer == nullptr) {
					throw invalid_iterator();
				}
				return this->pointer->data;
			}

			const value_type* operator->() const {
				if (pointer == nullptr) {
					throw invalid_iterator();
				}
				return &(this->pointer->data);
			}

			const_iterator operator++(int) {
				if (pointer == belong->watcher) {
					throw invalid_iterator();
				}
				const_iterator tmp(*this);
				pointer = find_next(pointer);
				return tmp;
			}
			/**
			* TODO ++iter
			*/
			const_iterator & operator++() {
				if (pointer == belong->watcher) {
					throw invalid_iterator();
				}
				pointer = find_next(pointer);
				return *this;
			}
			/**
			* TODO iter--
			*/
			const_iterator operator--(int) {
				if (pointer == belong->watcher) {
					throw invalid_iterator();
				}
				const_iterator tmp(*this);
				if (pointer == nullptr) {
					pointer = back();
				} else {
					pointer = find_prev(pointer);
				}
				return tmp;
			}
			/**
			* TODO --iter
			*/
			const_iterator & operator--() {
				if (pointer == belong->watcher) {
					throw invalid_iterator();
				}
				if (pointer == nullptr) {
					pointer = back();
				} else {
					pointer = find_prev(pointer);
				}
				return *this;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			bool operator==(const iterator &rhs) const {
				if (belong == rhs.belong) {
					if (pointer == belong->watcher && rhs.pointer == nullptr) {
						return true;
					} else if (pointer == nullptr && rhs.pointer == belong->watcher) {
						return true;
					}
				} else {
					return false;
				}
				return rhs.pointer == this->pointer;
			}
			bool operator==(const const_iterator &rhs) const {
				if (belong == rhs.belong) {
					if (pointer == belong->watcher && rhs.pointer == nullptr) {
						return true;
					} else if (pointer == nullptr && rhs.pointer == belong->watcher) {
						return true;
					}
				} else {
					return false;
				}
				return rhs.pointer == this->pointer;
			}
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const {
				return !(*this == rhs);
			}
			bool operator!=(const const_iterator &rhs) const {
				return !(*this == rhs);
			}
	};
	/**
	 * TODO two constructors
	 */
	map() {
		root = nullptr;
		tree_size = 0;
		watcher = operator new (sizeof(node));
	}
	map(const map &other) {
		root = Copy(other.root, nullptr);
		tree_size = other.tree_size;
		watcher = operator new (sizeof(node));
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		if (&other == this) {
			return *this;
		}
		Delete(root);
		root = Copy(other.root, nullptr);
		tree_size = other.tree_size;
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {
		Delete(root);
		operator delete (watcher);
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		iterator res = find(key);
		if (res == end()) {
			throw index_out_of_bound();
		}
		return res->second;
	}
	const T & at(const Key &key) const {
		const_iterator res = find(key);
		if (res == cend()) {
			throw index_out_of_bound();
		}
		return res->second;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		iterator res = find(key);
		if (res == end()) {
			value_type new_element(key, T());
			return insert(new_element).first->second;
		}
		return res->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		const_iterator res = find(key);
		if (res == cend()) {
			throw index_out_of_bound();
		}
		return res->second;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() const {
		if (root == nullptr) {
			return iterator((node*)watcher, this);
		}
		node *p = root;
		while (p->left_child != nullptr) {
			p = p->left_child;
		}
		return iterator(p, this);
	}
	const_iterator cbegin() const {
		return const_iterator(begin());
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() const {
		return iterator(nullptr, this);
	}
	const_iterator cend() const {
		return const_iterator(end());
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
		return !tree_size;
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
		return tree_size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		Delete(root);
		root = nullptr;
		tree_size = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		node* p = root, *bro = root;
		if (root == nullptr) {
			tree_size++;
			p = new node(value);
			root = p;
			return pair(iterator(p, this), true);
		}
		int flag = 0;
		while (!flag) {
			if (comp(value.first, p->data.first)) {
				if (p->left_child != nullptr) {
					bro = p->right_child;
					p = p->left_child;
				} else {
					p->left_child = new node(value, p, nullptr, nullptr);
					p = p->left_child;
					flag = 1;
				}
			} else if (comp(p->data.first, value.first)) {
				if (p->right_child != nullptr) {
					bro = p->left_child;
					p = p->right_child;
				} else {
					p->right_child = new node(value, p, nullptr, nullptr);
					p = p->right_child;
					flag = 1;
				}
			} else {
				flag = 2;
			}
		}
		if (flag == 2) {
			return pair(iterator(p, this), false);
		} else {
			tree_size++;
			InsertCheck(p);
			return pair(iterator(p, this), true);
		}
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
		if (pos.belong != this) {
			throw invalid_iterator();
		}
		if (pos == end()) {
			throw invalid_iterator();
		}
		node *p = pos.pointer, *tmp = pos.pointer;
		if (tmp->left_child != nullptr) {
			tmp = tmp->left_child;
			while (tmp->right_child != nullptr) {
				tmp = tmp->right_child;
			}
		}
		if (tmp == p) {
			erase(tmp->data.first);
			ChangeSon(p, nullptr);
			delete p;
			tree_size--;
			return;
		}
		erase(tmp->data.first);
		tmp->color = p->color; tmp->father = p->father; tmp->left_child = p->left_child; tmp->right_child = p->right_child;
		ChangeSon(p, tmp);
		ChangeFather(tmp);
		ChangeRoot(p, tmp);
		delete p;
		tree_size--;
		return;
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
		return find(key) == end() ? 0 : 1;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		node *p = root;
		while (p != nullptr && (comp(key, p->data.first) || comp(p->data.first, key))) {
			if (comp(key, p->data.first)) {
				p = p->left_child;
			} else {
				p = p->right_child;
			}
		}
		return iterator(p, this);
	}
	const_iterator find(const Key &key) const {
		node *p = root;
		while (p != nullptr && (comp(key, p->data.first) || comp(p->data.first, key))) {
			if (comp(key, p->data.first)) {
				p = p->left_child;
			} else {
				p = p->right_child;
			}
		}
		return const_iterator(p, this);
	}

private:

	void erase(Key key) {
		node *p = root, *bro = root;
		while (p != nullptr && (comp(key, p->data.first) || comp(p->data.first, key))) {
			EraseAdjust(p, bro, key);
			if (comp(key, p->data.first)) {
				bro = p->right_child;
				p = p->left_child;
			} else if (comp(p->data.first, key)) {
				bro = p->left_child;
				p = p->right_child;
			}
		}
		if (p->color != RED) {
			if (p->left_child != nullptr) {
				p->left_child->father = p->father;
				p->left_child->color = BLACK;
				ChangeSon(p, p->left_child);
			} else if (p->right_child != nullptr) {
				p->right_child->father = p->father;
				p->right_child->color = BLACK;
				ChangeSon(p, p->right_child);
			} else {
				if (COLOR(bro->left_child) == BLACK && COLOR(bro->right_child) == BLACK) {
					if (p->father != nullptr) {
						p->father->color = BLACK;
					}
					p->color = RED;
					bro->color = RED;
				} else if (COLOR(OUTWARD(bro)) == RED) {
					if (bro->father->left_child == bro) {
						RightRotate(bro);
					} else {
						LeftRotate(bro);
					}
					bro->color = p->color = RED;
					bro->left_child->color = bro->right_child->color = BLACK;
				} else {
					if (bro->father->left_child == bro) {
						LeftRotate(bro->right_child);
						RightRotate(bro->father);
					} else {
						RightRotate(bro->left_child);
						LeftRotate(bro->father);
					}
					p->color = bro->father->color = RED;
					bro->color = p->father->color = BLACK;
				}
				ChangeSon(p, nullptr);
			}
		}
		ChangeFather(p->father);
		if (p->left_child != nullptr) {
			ChangeRoot(p, p->left_child);
			ChangeSon(p, p->left_child);
		} else {
			ChangeRoot(p, p->right_child);
			ChangeSon(p, p->right_child);
		}
		return;
	}

	void EraseAdjust(node *p, node *bro, Key key) {	
		if (p->color == RED) {
			return;
		}
		if (COLOR(p->left_child) == BLACK && COLOR(p->right_child) == BLACK) {
			if (COLOR(bro->left_child) == BLACK && COLOR(bro->right_child) == BLACK) {
				if (p->father != nullptr) {
					p->father->color = BLACK;
				}
				p->color = RED;
				bro->color = RED;
			} else if (COLOR(OUTWARD(bro)) == RED) {
				if (bro->father->left_child == bro) {
					RightRotate(bro);
				} else {
					LeftRotate(bro);
				}
				bro->color = p->color = RED;
				bro->left_child->color = bro->right_child->color = BLACK;
			} else {
				if (bro->father->left_child == bro) {
					LeftRotate(bro->right_child);
					RightRotate(bro->father);
				} else {
					RightRotate(bro->left_child);
					LeftRotate(bro->father);
				}
				p->color = bro->father->color = RED;
				bro->color = p->father->color = BLACK;
			}
		} else {
			if (comp(key, p->data.first)) {
				bro = p->right_child;
				p = p->left_child;
			} else {
				bro = p->left_child;
				p = p->right_child;
			}
			if (p->color != RED) {
				if (p->father->left_child == p) {
					LeftRotate(bro);
				} else {
					RightRotate(bro);
				}
				p->father->color = RED;
				bro->color = BLACK;
			}
		}
	}

	void InsertCheck(node *target) {
		if (COLOR(target->father) == BLACK) {
			return;
		} else {
			if (target->father->father != nullptr) {
				if (COLOR(target->father->father->left_child) == COLOR(target->father->father->right_child)) {
					target->father->father->left_child->color = target->father->father->right_child->color = BLACK;
					target->father->father->color = RED;
					InsertCheck(target->father->father);
				} else {
					if (target == INWARD(target->father)) {
						if (target == target->father->right_child) {
							LeftRotate(target);
							RightRotate(target);
						} else {
							RightRotate(target);
							LeftRotate(target);
						}
						target->color = BLACK;
						target->left_child->color = target->right_child->color = RED;
					} else {
						if (target == target->father->left_child) {
							RightRotate(target->father);
						} else {
							LeftRotate(target->father);
						}
						target->father->left_child->color = target->father->right_child->color = RED;
						target->father->color = BLACK;
					}
				}
			} else {
				target->father->color = BLACK;
			}
		}
		return;
	}

	node* Copy(const node *root, node *father) {
		if (root == nullptr) {
			return nullptr;
		}
		node *tmp;
		tmp = new node(root->data, father, nullptr, nullptr);
		tmp->color = root->color;
		node *l = Copy(root->left_child, tmp), *r = Copy(root->right_child, tmp);
		tmp->left_child = l; tmp->right_child = r;
		return tmp;
	}

	void Delete(node *root) {
		if (root == nullptr) {
			return;
		}
		Delete(root->left_child);
		Delete(root->right_child);
		delete root;
		return;
	}
	static node *find_next(node *target) {
		if (target == nullptr) {
			throw invalid_iterator();
		}
		if (target->right_child != nullptr) {
			target = target->right_child;
			while (target->left_child != nullptr) {
				target = target->left_child;
			}
		} else {
			node *tmp = target->father;
			while (tmp != nullptr && tmp->left_child != target) {
				target = tmp;
				tmp = tmp->father;
			}
			target = tmp;
		}

		return target;
	}

	static node *find_prev(node *target) {
		if (target->left_child != nullptr) {
			target = target->left_child;
			while (target->right_child != nullptr) {
				target = target->right_child;
			}
		} else {
			node *tmp = target->father;
			while (tmp->right_child != target) {
				target = tmp;
				tmp = tmp->father;
			}
			target = tmp;
		}
		if (target == nullptr) {
			throw invalid_iterator();
		}
		return target;
	}

	void ChangeFather(node *target) {
		if (target == nullptr) {
			return;
		}
		if (target->left_child != nullptr) {
			target->left_child->father = target;
		}
		if (target->right_child != nullptr) {
			target->right_child->father = target;
		}
		return;
	}

	void ChangeSon(node *target, node *son) {
		if (target->father == nullptr) {
			return;
		}
		if (target->father->left_child == target) {
			target->father->left_child = son;
		} else if (target->father->right_child == target) {
			target->father->right_child = son;
		}
		return;
	}

	void RightRotate(node *target) {
		target->father->left_child = target->right_child;
		target->right_child = target->father;
		target->father = target->right_child->father;
		ChangeRoot(target->right_child, target);
		ChangeSon(target->right_child, target);
		ChangeFather(target);
		ChangeFather(target->right_child);
		return;
	}

	void LeftRotate(node *target) {
		target->father->right_child = target->left_child;
		target->left_child = target->father;
		target->father = target->left_child->father;
		ChangeRoot(target->left_child, target);
		ChangeSon(target->left_child, target);
		ChangeFather(target);
		ChangeFather(target->left_child);
		return;
	}

	void ChangeRoot(node *old_root, node *new_root) {
		if (old_root == root) {
			root = new_root;
		}
		return;
	}

private:
	node *root;
	void *watcher;
	size_t tree_size;
	Compare comp;
};

}

#endif
