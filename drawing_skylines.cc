// For Google Phone Interview.
// Author: kevinew.1221@gmail.com (Wenkai Liu)
// Date: 2014.3.27

/* Description:
   窗外有很多矩形建筑物，站在窗口用相机拍一张照片，求出照片中许多建筑物重叠后出现的锯齿状的外形轮廓。
   Analyze:
   Using the segment tree to solve it.
 */

#include<iostream>

using namespace std;

struct Node {
  int h, x, y;
  Node *left, *right;
} root;

void UpdateTree(Node * r, Node* line) {
  if (NULL == line || line.y - line.x == 0) return;
  if (NULL == r) *r = line;

  // 两条线段左右两端完全对qi的情况
  if (r->x == line->x && r->y == line->y ) {
    if (r->h < line->h) r->h = line->h;
    return;
  }

  // line和当前节点无重合
  if (line->x >= r->y) {
    UpdateTree(r->right, line);
    return;
  } else if (line->y <= r->x) {
    UpdateTree(r->left, line);
    return;
  }

  // longer or shorter compared to r left point.
  if (line->x < r->x) { // long
    Node* line1 = new Node(*line);
    line1->y = r->x;
    UpdateTree(r->left, line1);
    line->x = r->x;
    UpdateTree(r, line);
  } else if (line->x > r->x) { // short
    Node* node = new Node(*r);
    node->y = line->x;
    node->right = NULL;
    node->left = r->left;
    r->left = node;
    r->x = line->x;
    updateTree(r, line);
  }
  // longer or shorter compared to r right point.
  if (line->y > r->y) {
    Node* line1 = new Node(*line);
    line1->x = r->y;
    UpdateTree(r->right, line1);
    line->y = r->y;
    UpdateTree(r, line);
  } else if (line->y > r->y) {
    Node* node = new Node(*r);
    node->x = line->y;
    node->left = NULL;
    node->right = r->right;
    r->right = node;
    r->y = line->y;
    updateTree(r, line);
  }

}


void Insert(Node line) {
  Node *building = new Node(line);
  UpdateTree(r, building);
}

void Output(Node *r) {
  if (NULL == r) return;
  Output(r->left);
  cout << r->x << " " << r->y << " " << r->h << endl;
  Output(r->right);
}

void Run(const vector<Node> & building) {
  for (int i = 0; i < building.size(); ++i) {
    Insert(building[i]);
  }

  Output(r);
  return;
}

int main(int argc, char **argv) {
  vector<Node> building;
  
  // TODO(wenkailiu): initialize building

  Run(building);

  return 0;
}

