#pragma once

#include "Ast/Expression.hpp"
#include "Ast/Item.hpp"
#include "Ast/Statement.hpp"
#include "Entity/Symbol.hpp"
#include <functional>
#include <unordered_map>
#include <variant>

struct SemanticAnalyzer;

enum struct TaskResult {
    Completed,
    Waiting,
    Errored
};

struct Task {
  public:
    using Payload  = std::variant<Item const*, Statement const*, Expression const*>;
    using Callback = std::function<TaskResult(SemanticAnalyzer&, Payload)>;

    enum struct TargetStage {        
        TypeCheck,
    };

    auto GetStage(this Task const&) -> TargetStage;

  private:
    SymbolId m_dependency;
    Callback m_callback;
    TargetStage m_targetStage;
};

struct TaskManager {
public:
    using TaskList = std::vector<Task>;
    using Pending  = std::unordered_map<usize, TaskList>;

    explicit TaskManager() = default;


    // Stage a callback to be executed when a symbol has been resolved
    auto WaitOn(this TaskManager &, SymbolId, Task::Callback) -> void;
    auto Flush(this TaskManager &) -> void;
private:
    Pending m_pendingTasks;
};