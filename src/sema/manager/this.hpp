#pragma once

#include "ast/expression.hpp"
#include "ast/item.hpp"
#include "ast/statement.hpp"
#include "entity/symbol.hpp"
#include <functional>
#include <unordered_map>
#include <variant>

struct SemanticAnalyzer;

enum struct TaskResult {
    Completed,
    Waiting,
};

struct Task {
  public:
    using Payload  = std::variant<Item const*, Statement const*, Expression const*>;
    using Callback = std::function<TaskResult(SemanticAnalyzer&, Payload)>;

    enum struct TargetStage {        
        TypeCheck,
    };

    auto get_stage(this Task const&) -> TargetStage;

  private:
    SymbolId m_dependency;
    Callback m_callback;
    TargetStage m_target_stage;
};

struct TaskManager {
public:
    using TaskList = std::vector<Task>;
    using Pending  = std::unordered_map<usize, TaskList>;

    explicit TaskManager() = default;


    // Stage a callback to be executed when a symbol has been resolved
    auto wait_on(this TaskManager &, SymbolId, Task::Callback) -> void;
    auto flush(this TaskManager &) -> void;
private:
    Pending m_pending_tasks;
};