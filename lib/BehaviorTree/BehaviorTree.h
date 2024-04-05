#ifndef BEHAVIOR_TREE_BEHAVIOR_TREE
#define BEHAVIOR_TREE_BEHAVIOR_TREE

#include <functional>
#include <vector>
#include <memory>

/**
 * @file BehaviorTree.h
 * @author massivebugs
 * @date March 10, 2024
 * @brief A super simple implementation of a Behavior Tree.
 * Reference: https://en.wikipedia.org/wiki/Behavior_tree_(artificial_intelligence,_robotics_and_control)
 *
 * Usage:
 * Implement your own Leaf Node like in the example below.
 * There are 3 Nodes for control flow as of now:
 * - Sequence
 * - Selector
 * - RandomSelector
 * - Condition
 *
 * Example Leaf Node:
 * auto node = BTLeafNode<T *>{
 *     [&](T *ctx)
 *     {
 *        // Initialization function
 *     },
 *     [&](T *ctx)
 *     {
 *        // Tick function
 *     },
 * };
 */

/**
 * @brief Status of Behavior Tree nodes.
 */
enum class BTNodeStatus
{
    Running,
    Success,
    Fail
};

/**
 * @brief The base interface for every Behavior Tree node.
 *
 * This class defines the interface that all Behavior Tree nodes must implement.
 * It provides methods for initialization and ticking of the nodes.
 *
 * @tparam T The type of the context used by the Behavior Tree nodes.
 */
template <typename T>
class IBTNode
{
public:
    virtual ~IBTNode() {}

    /**
     * @brief Initializes the Behavior Tree node.
     *
     * This method is called after a Behavior Tree returns either BTNodeStatus::Success
     * or BTNodeStatus::Fail. Implementations can use this method to set states and variables
     * to be used for the upcoming tick() calls.
     *
     * @param ctx The context used for initialization.
     */
    virtual void initialize(T ctx) = 0;

    /**
     * @brief Performs a tick operation on the Behavior Tree node.
     *
     * This method is called once every behavior iteration unless BTNodeStatus::Running is returned.
     *
     * @param ctx The context used for the tick operation.
     * @return BTNodeStatus The status of the tick operation.
     */
    virtual BTNodeStatus tick(T ctx) = 0;
};

/**
 * @brief The base interface for composite nodes in the Behavior Tree.
 *
 * This class contains common functionalities for all control flow nodes in the Behavior Tree.
 * It provides methods for adding child nodes and initializing them.
 *
 * @tparam T Context type.
 */
template <typename T>
class IBTCompositeNode
{
protected:
    // Index of the current child node being processed.
    int current;
    std::vector<std::unique_ptr<IBTNode<T>>> children;

public:
    /**
     * @brief Adds a child node.
     *
     * @param node
     */
    void addChildNode(std::unique_ptr<IBTNode<T>> node)
    {
        children.push_back(std::move(node));
    }

    /**
     * @brief Initializes all child nodes.
     *
     * @param ctx The context.
     */
    void initializeChildNodes(T ctx)
    {
        current = 0;
        for (auto &child : children)
        {
            child->initialize(ctx);
        }
    }
};

/**
 * @brief The base interface for decorator nodes in the Behavior Tree.
 *
 * This class contains common functionalities for all decorator nodes in the Behavior Tree.
 *
 * @tparam T Context type.
 */
template <typename T>
class IBTDecoratorNode
{
protected:
    // Flag indicating whether the decorator node is currently running.
    bool isRunning;
    std::unique_ptr<IBTNode<T>> child;

public:
    /**
     * @brief Sets the child node.
     *
     * @param node
     */
    void setChildNode(std::unique_ptr<IBTNode<T>> node)
    {
        child = std::move(node);
    }

    /**
     * @brief Initializes the child node.
     *
     * @param ctx Context.
     */
    void initializeChildNode(T ctx)
    {
        isRunning = false;
        child->initialize(ctx);
    }
};

/**
 * @brief BTSequenceNode runs all child Nodes until any fails.
 *
 * This class represents a sequence node in a behavior tree.
 * It executes its child nodes in order, stopping when any child node returns a failure status.
 *
 * @tparam T Context type.
 */
template <typename T>
class BTSequenceNode : public IBTCompositeNode<T>, public IBTNode<T>
{
public:
    /**
     * @brief Initializes the node and it's children.
     *
     * @param ctx Context.
     */
    void initialize(T ctx) override
    {
        this->initializeChildNodes(ctx);
    }

    /**
     * @brief Performs a tick operation on the sequence node.
     *
     * Executes the child nodes of the sequence node in order.
     * Stops when any child node returns a status other than BTNodeStatus::Success.
     *
     * @param ctx Context.
     * @return BTNodeStatus The status of the tick operation.
     */
    BTNodeStatus tick(T ctx) override
    {
        while (this->current < this->children.size())
        {
            BTNodeStatus childStatus = this->children[this->current]->tick(ctx);
            if (childStatus != BTNodeStatus::Success)
            {
                return childStatus;
            }
            this->current++;
        }
        return BTNodeStatus::Success;
    }
};

/**
 * @brief BTSelectorNode runs all child Nodes until any succeeds.
 *
 * This class represents a selector node in a behavior tree.
 * It executes its child nodes in order, stopping when any child node returns a success status.
 *
 * @tparam T Context type.
 */
template <typename T>
class BTSelectorNode : public IBTCompositeNode<T>, public IBTNode<T>
{
public:
    /**
     * @brief Initializes the node and it's children.
     *
     * @param ctx Context.
     */
    void initialize(T ctx) override
    {
        this->initializeChildNodes(ctx);
    }

    /**
     * @brief Performs a tick operation.
     *
     * Executes the child nodes of the selector node in order.
     * Stops when any child node returns a status other than BTNodeStatus::Fail.
     *
     * @param ctx Context.
     * @return BTNodeStatus The status of the tick operation.
     */
    BTNodeStatus tick(T ctx) override
    {
        while (this->current < this->children.size())
        {
            BTNodeStatus childStatus = this->children[this->current]->tick(ctx);
            if (childStatus != BTNodeStatus::Fail)
            {
                return childStatus;
            }
            this->current++;
        }
        return BTNodeStatus::Fail;
    }
};

/**
 * @brief BTRandomSelectorNode shuffles child Nodes on initialization, then runs all child Nodes until any succeeds.
 *
 * This class represents a random selector node in a behavior tree.
 * It shuffles its child nodes on initialization, then executes them in order.
 * Stops when any child node returns a status other than BTNodeStatus::Fail.
 *
 * @tparam T Context type.
 */
template <typename T>
class BTRandomSelectorNode : public IBTCompositeNode<T>, public IBTNode<T>
{
public:
    /**
     * @brief Initializes the random selector node.
     *
     * Shuffles children, and initializes them.
     *
     * @param ctx Context.
     */
    void initialize(T ctx) override
    {
        std::random_shuffle(this->children.begin(), this->children.end());
        this->initializeChildNodes(ctx);
    }

    /**
     * @brief Performs a tick operation on the random selector node.
     *
     * Executes the child nodes of the random selector node in order. Stops when any child node
     * returns a status other than BTNodeStatus::Fail.
     *
     * @param ctx Context.
     * @return BTNodeStatus The status of the tick operation.
     */
    BTNodeStatus tick(T ctx) override
    {
        while (this->current < this->children.size())
        {
            BTNodeStatus childStatus = this->children[this->current]->tick(ctx);
            if (childStatus != BTNodeStatus::Fail)
            {
                return childStatus;
            }
            this->current++;
        }
        return BTNodeStatus::Fail;
    }
};

/**
 * @brief BTConditionNode runs its child only if the checkFunc returns true.
 *
 * This class represents a condition node in a behavior tree.
 * It checks a condition using a provided function, then executes its child node only if the condition is true.
 *
 * @tparam T Context type.
 */
template <typename T>
class BTConditionNode : public IBTDecoratorNode<T>, public IBTNode<T>
{
private:
    std::function<bool(T)> checkFunc;

public:
    /**
     * @brief Constructs a new BTConditionNode object.
     *
     * @param func The function used to check the condition.
     */
    BTConditionNode(std::function<bool(T)> func) : checkFunc(func) {}

    /**
     * @brief Initializes the condition node and it's child.
     *
     * @param ctx Context.
     */
    void initialize(T ctx) override
    {
        this->initializeChildNode(ctx);
    }

    /**
     * @brief Performs a tick operation on the condition node.
     *
     * Checks the condition using the provided function.
     * If the condition is true, executes the child node.
     * Otherwise, returns BTNodeStatus::Fail.
     *
     * @param ctx Context.
     * @return BTNodeStatus The status of the tick operation.
     */
    BTNodeStatus tick(T ctx) override
    {
        if (this->isRunning || checkFunc(ctx))
        {
            BTNodeStatus state = this->child->tick(ctx);
            this->isRunning = state == BTNodeStatus::Running;
            return state;
        }
        return BTNodeStatus::Fail;
    }
};

/**
 * @brief A generic leaf node of the behavior tree.
 *
 * This class represents a leaf node in a behavior tree.
 * It performs specific tasks and does not have children.
 *
 * @tparam T The type of the context used for tick operations.
 */
template <typename T>
class BTLeafNode : public IBTNode<T>
{
public:
    /**
     * @brief Constructs a new BTLeafNode object.
     *
     * @param initializeFunc The function used to initialize the node.
     * @param tickFunc The function used to perform the tick operation.
     */
    BTLeafNode(std::function<void(T)> initializeFunc, std::function<BTNodeStatus(T)> tickFunc) : initializeFunc{initializeFunc},
                                                                                                 tickFunc{tickFunc} {}
    /**
     * @brief Initializes the leaf node.
     *
     * @param ctx Context.
     */
    void initialize(T ctx) override
    {
        initializeFunc(ctx);
    }

    /**
     * @brief Performs a tick operation on the leaf node.
     *
     * @param ctx Context.
     * @return BTNodeStatus The status of the tick operation.
     */
    BTNodeStatus tick(T ctx) override
    {
        return tickFunc(ctx);
    }

private:
    std::function<void(T)> initializeFunc;
    std::function<BTNodeStatus(T)> tickFunc;
};

#endif
