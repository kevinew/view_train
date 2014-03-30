// For Google Phone Interview.
// Author: kevinew.1221@gmail.com (Wenkai Liu)
// Date: 2014.3.27

// Description:
//   窗外有很多矩形建筑物，站在窗口用相机拍一张照片，求出照片中许多建筑物重叠后出现的锯齿状的外形轮廓。
// Analyze:
//   Using the binary tree or segment tree to solve it.
// OJ:  http://codeforces.com/contest/35/problem/E

#include<stdio.h>
#include<vector>
#include<iostream>

using namespace std;

typedef struct Point {
  int h, x;
} POINT;
vector<POINT> surface, raw_surface;

struct Node {
  Node():left(NULL), right(NULL) {}
  ~Node() {}
  int h, x, y;
  Node *left, *right;
} *root;

void UpdateTree(Node** r, Node* line) {
  if (NULL == line || line->y - line->x == 0) return;
  if (NULL == *r) {
    *r = line;
    return;
  }
  // 两条线段左右两端完全对qi的情况
  if ((*r)->x == line->x && (*r)->y == line->y ) {
    if ((*r)->h < line->h) (*r)->h = line->h;
    return;
  }

  // line和当前节点无重合
  if (line->x >= (*r)->y) {
    UpdateTree(&((*r)->right), line);
    return;
  } else if (line->y <= (*r)->x) {
    UpdateTree(&((*r)->left), line);
    return;
  }

  // longer or shorter compared to r left point.
  if (line->x < (*r)->x) { // long
    Node* line1 = new Node(*line);
    line1->y = (*r)->x;
    UpdateTree(&((*r)->left), line1);
    line->x = (*r)->x;
    UpdateTree(r, line);
  } else if (line->x > (*r)->x) { // short
    Node* node = new Node(*(*r));
    node->y = line->x;
    node->right = NULL;
    node->left = (*r)->left;
    (*r)->left = node;
    (*r)->x = line->x;
    UpdateTree(r, line);
  }
  // longer or shorter compared to r right point.
  if (line->y > (*r)->y) {
    Node* line1 = new Node(*line);
    line1->x = (*r)->y;
    UpdateTree(&((*r)->right), line1);
    line->y = (*r)->y;
    UpdateTree(r, line);
  } else if (line->y > (*r)->y) {
    Node* node = new Node(*(*r));
    node->x = line->y;
    node->left = NULL;
    node->right = (*r)->right;
    (*r)->right = node;
    (*r)->y = line->y;
    UpdateTree(r, line);
  }

}


void Insert(Node line) {
  Node *building = new Node(line);
  UpdateTree(&root, building);
}

// Extract result - inorder traversal.
void SetResult(Node *r) {
  if (NULL == r) return;

  SetResult(r->left);
  if (surface.size() >= 2 && surface.back().h == r->h) {
    surface.resize(surface.size() - 1);
    POINT point_r;
    point_r.x = r->y;
    point_r.h = r->h;
    surface.push_back(point_r);
  } else {
    POINT point_l;
    point_l.x = r->x;
    point_l.h = r->h;
    surface.push_back(point_l);
    POINT point_r;
    point_r.x = r->y;
    point_r.h = r->h;
    surface.push_back(point_r);
  }
  SetResult(r->right);
}

void NormalizeResult() {
  // When there is blank space between 2 building.
  vector<POINT>::iterator it;
  // for (int i = 0; i < surface.size(); ++i) {
  int cnt = 0;
  for (it = surface.begin(); it != surface.end(); ++it, ++cnt) {
    if (0 == cnt) continue;
    if (cnt & 1) {
      if (it + 1 != surface.end()  && it->x != (it + 1)->x) {
        POINT point1;
        point1.x = it->x;
        point1.h = 0;
        surface.insert(it + 1, point1);
        POINT point2;
        point2.x = (it + 2)->x;
        point2.h = 0;
        surface.insert(it + 2, point2);
        cnt += 2;
        it += 2;
      }
    }
  }

  if (surface.front().h > 0) {
    POINT point;
    point.x = surface.front().x;
    point.h = 0;
    surface.insert(surface.begin(), point);
  }
  if (surface.back().h > 0) {
    POINT point;
    point.x = surface.back().x;
    point.h = 0;
    surface.push_back(point);
  }

}

void Run(vector<Node> buildings) {
  for (int i = 0; i < buildings.size(); ++i) {
    Insert(buildings[i]);
  }

  SetResult(root);
  NormalizeResult();

  // cout << surface.size() << endl;
  printf("%d\n", (int)surface.size());
  for (int i = 0; i < surface.size(); ++i) {
    printf("%d %d\n", surface[i].x, surface[i].h);
    // cout << surface[i].x << " " << surface[i].h << endl;
  }
}

int main(int argc, char **argv) {
  vector<struct Node> buildings;
  
  // TODO(wenkailiu): refactor
  int n;
  if (EOF != scanf("%d", &n)) {
    for (int i = 0; i < n; ++i) {
      Node building;
      scanf("%d%d%d",&building.h, &building.x, &building.y);
      if (building.x > building.y) {
        int tmp = building.x;
        building.x = building.y;
        building.y = tmp;
      }
      buildings.push_back(building);
    }
    Run(buildings);
  }

  return 0;
}
