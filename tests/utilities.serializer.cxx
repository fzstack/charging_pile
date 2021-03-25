#ifdef TEST_SERIALIZER

#include <utilities/serializer.hxx>
#include <utilities/deserializer.hxx>
#include <utilities/memory_iostream.hxx>
#include <utilities/f.hxx>

using namespace std;

struct TNode {
    rt_uint8_t value;
    weak_ptr<TNode> parent;
    shared_ptr<TNode> lchild, rchild;
    weak_ptr<TNode> prev;
    shared_ptr<TNode> next;
};

static void test_ser_tree() {
    auto root = std::make_shared<TNode>(TNode{0});
    auto lchild = std::make_shared<TNode>(TNode{10});
    auto rchild = std::make_shared<TNode>(TNode{11});

    root->lchild = lchild;
    lchild->parent = root;
    root->rchild = rchild;
    rchild->parent = root;
    lchild->next = rchild;
    rchild->prev = lchild;

    F{} << "before: "_r << root << endln;
    auto stream = make_shared<MemoryIOStream>();
    Serializer{stream}.build(root);
    F{} << "data: "_r << stream->getBuffer() << endln;
    auto des = Deserializer{stream}.parse<decltype(root)>();
    F{} << "after: "_r << des << endln;
}

struct Vertex {
    rt_uint8_t value;
    std::list<std::weak_ptr<Vertex>> outs;
};

static void test_ser_graph() {
    auto stream = make_shared<MemoryIOStream>();
    {
        auto v = map<int, std::shared_ptr<Vertex>>{};
        for(rt_uint8_t i = 1; i <= 4; i++) {
            v[i] = make_shared<Vertex>(Vertex{i});
        }
        v[1]->outs = {v[2], v[3], v[4]};
        v[2]->outs = {v[1], v[4], v[3]};
        v[3]->outs = {v[1], v[2], v[4]};
        v[4]->outs = {v[1], v[2], v[4]};

        F{}.println(v[1]);
        Serializer{stream}.build(v[1]);
    }
    F{}.println(stream->getBuffer());
    {
        auto des = Deserializer{stream};
        auto result = des.parse<weak_ptr<Vertex>>();
        F{}.println(result);
    }
}

MSH_CMD_EXPORT(test_ser_graph, );

MSH_CMD_EXPORT(test_ser_tree, );


#endif
