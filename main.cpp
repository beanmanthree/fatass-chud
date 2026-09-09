#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct SGT {
    struct Node {
        ll x{0};
        int l{-1}, r{-1};
        ll a{0};
        Node() {}
        Node (ll x) : x{x} {}
        Node (ll x, int l, int r) : x{x}, l{l}, r{r} {}
    };
    static inline Node tree[40 * 100000]{};
    static inline int root[100001]{};
    int ptr{0};
    int t{-1};
    const int n{};
    SGT(int n, vector<int>& arr) : n{n} {
        root[++t] = build(arr, 0, n - 1);
    }
    int build(vector<int>& arr, int l, int r) {
        const int v{ptr++};
        if (l == r) {
            tree[v] = Node{arr[l]};
            return v;
        }
        const int m{(l + r) / 2};
        const int lv{build(arr, l, m)};
        const int rv{build(arr, m + 1, r)};
        tree[v] = Node{tree[lv].x + tree[rv].x, lv, rv};
        return v;
    }
    int upd(int ql, int qr, ll a) {
        return upd(ql, qr, a, root[t], 0, n - 1);
    }
    int upd(int ql, int qr, ll a, int v, int l, int r) {
        if (r < ql || l > qr) return v;
        const int nv{ptr++};
        tree[nv] = tree[v];
        if (ql <= l && r <= qr) {
            tree[nv].x += (r - l + 1) * a;
            tree[nv].a += a;
            return nv;
        }
        const int m{(l + r) / 2};
        tree[nv].l = upd(ql, qr, a, tree[nv].l, l, m);
        tree[nv].r = upd(ql, qr, a, tree[nv].r, m + 1, r);
        tree[nv].x = tree[tree[nv].l].x + tree[tree[nv].r].x + tree[nv].a * (r - l + 1);
        return nv;
    }
    ll qry(int ql, int qr, int v) {
        return qry(ql, qr, v, 0, 0, n - 1);
    }
    ll qry(int ql, int qr, int v, ll a, int l, int r) {
        if (r < ql || l > qr) return 0;
        if (ql <= l && r <= qr) return tree[v].x + a * (r - l + 1);
        const int m{(l + r) / 2};
        a += tree[v].a;
        return qry(ql, qr, tree[v].l, a, l, m) + qry(ql, qr, tree[v].r, a, m + 1, r);
    }
};

int main() {
    cin.tie(0);
    cout.tie(0);
    ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    vector<int> arr(n);
    for (int i{0}; i < n; ++i) cin >> arr[i];
    SGT sgt{n, arr};
    for (int i{0}; i < m; ++i) {
        char c;
        cin >> c;
        switch (c) {
            int l, r, d, t;
            case 'C':
                cin >> l >> r >> d;
                --l, --r;
                sgt.root[++sgt.t] = sgt.upd(l, r, d);
                break;
            case 'Q':
                cin >> l >> r;
                --l, --r;
                cout << sgt.qry(l, r, sgt.root[sgt.t]) << '\n';
                break;
            case 'H':
                cin >> l >> r >> t;
                --l, --r;
                cout << sgt.qry(l, r, sgt.root[t]) << '\n';
                break;
            default:
                cin >> t;
                sgt.t = t;
                break;
        }
    }
}
