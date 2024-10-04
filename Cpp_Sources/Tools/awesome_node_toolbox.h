#ifndef AWESOMENODETOOLBOX_H
#define AWESOMENODETOOLBOX_H

#include <type_traits>
#include <vector>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

namespace ant
{

template <class ChildNodeClass>
ChildNodeClass* add_new_child(Node* ptrParentNode) {
    static_assert(std::is_convertible<ChildNodeClass, Node>::value);
    ChildNodeClass* ptrChildNode = memnew(ChildNodeClass);

    ptrParentNode->add_child(ptrChildNode);
    return ptrChildNode;
}

template <class ChildNodeClass>
std::vector<ChildNodeClass*> add_new_children(Node* ptrParentNode, uint nb) {
    static_assert(std::is_convertible<ChildNodeClass, Node>::value);
    std::vector<ChildNodeClass*> vectorPtrChildNode;

    for (int i = 0; i < nb; i++) {
        ChildNodeClass* ptrChildNode = memnew(ChildNodeClass);
        ptrParentNode->add_child(ptrChildNode);
        vectorPtrChildNode.push_back(ptrChildNode);
    }
    return vectorPtrChildNode;
}

template <class ChildNodeClass>
ChildNodeClass* load_packed_scene_as_new_child(Node* ptrParentNode, const String path) {
    static_assert(std::is_convertible<ChildNodeClass, Node>::value);
    ResourceLoader* ptrRessourceLoader = ResourceLoader::get_singleton();
    if (!ptrRessourceLoader->exists(path))
        return nullptr;

    Ref<PackedScene> packedScene = static_cast<Ref<PackedScene>>(ptrRessourceLoader->load(path));
    Node* ptrInstanciatedPackedScene = packedScene->instantiate();

    if (!ptrInstanciatedPackedScene)
        return nullptr;

    ChildNodeClass* ptrChildNode;
    ptrChildNode = Object::cast_to<ChildNodeClass>(ptrInstanciatedPackedScene);

    if (!ptrChildNode)
        return nullptr;

    ptrParentNode->add_child(ptrChildNode);
    return ptrChildNode;
}

template <class ChildNodeClass>
ChildNodeClass* get_first_child(Node* ptrParentNode) {
    static_assert(std::is_convertible<ChildNodeClass, Node>::value);
    std::vector<ChildNodeClass*> vectorPtrChildNode;
    Array arrChildren = ptrParentNode->get_children();

    for (int i = 0; i < arrChildren.size(); i++) {
        ChildNodeClass* ptrCurrentChildNode = Object::cast_to<ChildNodeClass>(arrChildren[i]);
        if (ptrCurrentChildNode)
            return ptrCurrentChildNode;
    }

    return nullptr;
}

template <class ChildNodeClass>
std::vector<ChildNodeClass*> get_children(Node* ptrParentNode) {
    static_assert(std::is_convertible<ChildNodeClass, Node>::value);
    std::vector<ChildNodeClass*> vectorPtrChildNode;
    Array arrChildren = ptrParentNode->get_children();

    for (int i = 0; i < arrChildren.size(); i++) {
        ChildNodeClass* ptrCurrentChildNode = Object::cast_to<ChildNodeClass>(arrChildren[i]);
        if (ptrCurrentChildNode)
            vectorPtrChildNode.push_back(ptrCurrentChildNode);
    }

    return vectorPtrChildNode;
}

template <class ChildNodeClass>
void delete_all_childs(Node* ptrParentNode) {
    static_assert(std::is_convertible<ChildNodeClass, Node>::value);
    std::vector<ChildNodeClass*> vectorPtrChildNode = ant::get_children<ChildNodeClass>(ptrParentNode);

    for (ChildNodeClass* ptrChildNode : vectorPtrChildNode) {
        ptrParentNode->remove_child(ptrChildNode);
        ptrChildNode->queue_free();
    }
}

}

#endif