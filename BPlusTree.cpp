#include "BPlusTree.h"
using std::cin;
using std::cout;

bool operator==(const Data &x, const Data &y) {
  return strcmp(x.key, y.key) == 0 && x.value == y.value;
}

bool operator<(const Data &x, const Data &y) {
  const int cmp = strcmp(x.key, y.key);
  if (cmp < 0)return true;
  if (cmp > 0)return false;
  return x.value < y.value;
}

bool operator<=(const Data &x, const Data &y) {
  return x < y || x == y;
}

bool operator>(const Data &x, const Data &y) {
  const int cmp = strcmp(x.key, y.key);
  if (cmp > 0)return true;
  if (cmp < 0)return false;
  return x.value > y.value;
}

bool operator>=(const Data &x, const Data &y) {
  return x > y || x == y;
}

int lower_bound(int L, int R, const Data &dt, const Node &nd) {
  //找到第一个>=dt的坐标，没有时返回尾端
  if (nd.data[R] < dt) {
    return R;
  }
  if (L >= R) {
    return L;
  }
  auto mid = (L + R) / 2;
  if (nd.data[mid] < dt) {
    return lower_bound(mid + 1, R, dt, nd);
  } else {
    return lower_bound(L, mid, dt, nd);
  }
}

void BPT::Split() {
  Node new_node;
  new_node.size = MAX_SIZE / 2;
  new_node.is_leaf = cur.is_leaf;
  new_node.parent = cur.parent;
  new_node.left = cur_idx; //分裂出的新节点在原节点右侧
  new_node.right = cur.right;
  cur.size = MAX_SIZE / 2;
  if (cur.is_leaf) {
    //叶分裂
    for (int i = 0; i < new_node.size; i++) {
      new_node.data[i] = cur.data[i + cur.size];
    }
    if (cur.parent != -1) {
      Node fa;
      tree.read(fa, cur.parent, 1);
      Data cur_up = cur.data[cur.size - 1];
      Data new_up = new_node.data[new_node.size - 1];
      int p = lower_bound(0, fa.size - 1, new_up, fa);
      if (fa.data[p] != new_up) {
        //不应出现这种情况
        std::cerr << "Leaf:cannot find origin!\n";
      } else {
        for (int i = fa.size - 1; i >= p; i--) {
          fa.data[i + 1] = fa.data[i];
          fa.child[i + 1] = fa.child[i];
        }
        fa.data[p] = cur_up;
        fa.child[p] = cur_idx;
        fa.data[p + 1] = new_up;
        fa.child[p + 1] = tree.push(new_node); //存入new_node的索引
        fa.size++;
        //维护叶节点链
        if (cur.right != -1) {
          Node tmp;
          tree.read(tmp, cur.right, 1);
          tmp.left = fa.child[p + 1];
          tree.write(tmp, cur.right, 1);
        }
        cur.right = fa.child[p + 1];
        tree.write(fa, cur.parent, 1);
        tree.write(cur, cur_idx, 1);
        if (fa.size == MAX_SIZE) {
          cur_idx = cur.parent;
          tree.read(cur, cur.parent, 1);
          Split();
        }
        return;
      }
    } else {
      //当前为根
      Node new_root;
      new_root.is_leaf = false;
      new_root.left = -1;
      new_root.right = -1;
      new_root.parent = -1;
      new_root.size = 2;
      root = tree.push(new_root); //得到新根
      cur.parent = new_node.parent = root;
      new_root.data[0] = cur.data[cur.size - 1];
      new_root.data[1] = new_node.data[new_node.size - 1];
      new_root.child[0] = cur_idx;
      new_root.child[1] = tree.push(new_node);
      cur.right = new_root.child[1];
      tree.write(new_root, root, 1);
      tree.write(cur, cur_idx, 1);
      return;
    }
  } else {
    //非叶分裂
    int new_idx = tree.push(new_node);
    for (int i = 0; i < new_node.size; i++) {
      new_node.data[i] = cur.data[i + cur.size];
      new_node.child[i] = cur.child[i + cur.size];
    }
    //修改父节点
    Node tmp;
    for (int i = 0; i < new_node.size; i++) {
      tree.read(tmp, new_node.child[i], 1);
      tmp.parent = new_idx;
      tree.write(tmp, new_node.child[i], 1);
    }
    if (cur.parent != -1) {
      Node fa;
      tree.read(fa, cur.parent, 1);
      Data cur_up = cur.data[cur.size - 1];
      Data new_up = new_node.data[new_node.size - 1];
      int p = lower_bound(0, fa.size - 1, new_up, fa);
      if (fa.data[p] != new_up) {
        std::cerr << "Not Leaf:cannot find origin!\n";
      } else {
        for (int i = fa.size - 1; i >= p; i--) {
          fa.data[i + 1] = fa.data[i];
          fa.child[i + 1] = fa.child[i];
        }
        fa.data[p] = cur_up;
        fa.child[p] = cur_idx;
        fa.data[p + 1] = new_up;
        fa.child[p + 1] = new_idx; //存入new_node的索引
        fa.size++;
        //维护叶节点链
        if (cur.right != -1) {
          Node tmp2;
          tree.read(tmp2, cur.right, 1);
          tmp2.left = new_idx;
          tree.write(tmp2, cur.right, 1);
        }
        cur.right = new_idx;
        tree.write(fa, cur.parent, 1);
        tree.write(cur, cur_idx, 1);
        tree.write(new_node, new_idx, 1);
        if (fa.size == MAX_SIZE) {
          cur_idx = cur.parent;
          tree.read(cur, cur.parent, 1);
          Split();
        }
        return;
      }
    } else {
      //当前为根
      Node new_root;
      new_root.is_leaf = false;
      new_root.left = -1;
      new_root.right = -1;
      new_root.parent = -1;
      new_root.size = 2;
      root = tree.push(new_root); //得到新根
      cur.parent = new_node.parent = root;
      new_root.data[0] = cur.data[cur.size - 1];
      new_root.data[1] = new_node.data[new_node.size - 1];
      new_root.child[0] = cur_idx;
      new_root.child[1] = new_idx;
      tree.write(new_root, root, 1);
      tree.write(cur, cur_idx, 1);
      tree.write(new_node, new_idx, 1);
      return;
    }
  }
}


void BPT::Insert(const Data &dt) {
  if (root == 0) {
    //空树
    Node new_node;
    new_node.size = 1;
    new_node.is_leaf = true;
    new_node.parent = -1;
    new_node.left = new_node.right = -1;
    new_node.data[0] = dt;
    root = tree.push(new_node);
    return;
  }
  cur_idx = root;
  tree.read(cur, root, 1);
  while (!cur.is_leaf) {
    int i = lower_bound(0, cur.size - 1, dt, cur);
    if (cur.data[i] < dt) {
      //插入最大Data时直接修改索引
      cur.data[i] = dt;
      tree.write(cur, cur_idx, 1);
    }
    cur_idx = cur.child[i];
    tree.read(cur, cur_idx, 1);
  }
  //cur到了叶节点
  int i = lower_bound(0, cur.size - 1, dt, cur);
  if (cur.data[i] == dt) {
    //同key同value
    return;
  }
  if (cur.data[i] < dt) {
    //插入最大
    cur.data[i + 1] = dt;
    cur.size++;
  } else {
    for (int k = cur.size - 1; k >= i; k--) {
      cur.data[k + 1] = cur.data[k];
    }
    cur.data[i] = dt;
    cur.size++;
  }
  tree.write(cur, cur_idx, 1);
  if (cur.size == MAX_SIZE) {
    Split();
  }
}
