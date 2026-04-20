#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    bool operator<(const Point &other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator==(const Point &other) const { return x == other.x && y == other.y; }
};

long long cross(const Point &a, const Point &b, const Point &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

vector<Point> convexHull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    if (p.size() <= 1) return p;

    vector<Point> lower, upper;
    for (auto &pt : p) {
        while (lower.size() >= 2 && cross(lower[lower.size() - 2], lower.back(), pt) <= 0) lower.pop_back();
        lower.push_back(pt);
    }
    for (int i = (int)p.size() - 1; i >= 0; i--) {
        auto &pt = p[i];
        while (upper.size() >= 2 && cross(upper[upper.size() - 2], upper.back(), pt) <= 0) upper.pop_back();
        upper.push_back(pt);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t; 
    while(1){
        int n;
        cin >> n;
        if(n==0) break;

        vector<Point> p(n);
        for (int i = 0; i < n; i++) cin >> p[i].x >> p[i].y;

        vector<Point> hull = convexHull(p);
        //cout << hull.size() << '\n';
        //for (auto &pt : hull) cout << pt.x << ' ' << pt.y << '\n';
        if(hull.size()!=n){
            cout<<"Yes"<<endl;
        }
        else{
            cout<<"No"<<endl;
        }

    }
        return 0;
}

