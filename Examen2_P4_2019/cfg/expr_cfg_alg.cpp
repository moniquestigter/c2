#include <sstream>
#include <string>
#include "expr_cfg.h"

extern CFGNodePool cfg_pool;

void CFGSetLabels(CFGStmt *root) {
    std::unordered_set<CFGStmt *> visitado;
    std::queue<CFGStmt *> qubo;
    int counter = 0;
    
    qubo.push(root);

    while(!qubo.empty()){

        CFGStmt *tempo = qubo.front();
        qubo.pop();

        if (visitado.find(tempo) != visitado.end()) {
            continue;
        }

        StdString labo = "LC" + std::to_string(counter++);
        tempo->label = labo;

        visitado.insert(tempo);
        CFGStmtVector edges = tempo->getEdges();

        for (int x=0; x<edges.size(); x++) {

            qubo.push(edges[x]);
        }
    }
}

std::string CFGToString(CFGStmt *root) 
{
    std::queue<CFGStmt *> q;
    std::unordered_set<CFGStmt *> visited;
    
    q.push(root);

    std::ostringstream out;
    while (!q.empty()) {
        CFGStmt *s = q.front();
        q.pop();

        if (visited.find(s) != visited.end()) {
            continue;
        }
        visited.insert(s);
        
        if (!s->label.empty()) {
            out << s->label << ": ";
        }
        out << s->toString();
        
        CFGStmtVector edges = s->getEdges();
        if (edges.size() == 1) {
            out << " : goto " << edges[0]->label;
        } else if (edges.size() == 0) {
            out << " : goto end"; 
        }
        out << '\n';

        for (int i = 0; i < edges.size(); i++) {
            q.push(edges[i]);
        }
    }
    out << "end:";

    return out.str();
}

static CFGStmt *CFGRemoveNopsHelper(CFGStmt *root, std::unordered_map<CFGStmt *, CFGStmt *>& visited) {
    if (root == nullptr) {
        return nullptr;
    }
    if (visited.find(root) != visited.end()) {
        return visited[root];
    }

    CFGStmt *stmt = root;
    if (root->isA(CFGNopStmt_kind)) {
        if (root->getNext() == nullptr) {
            visited[root] = root;
            return root;
        }
        do {
            stmt = stmt->getNext();
        } while (stmt->isA(CFGNopStmt_kind) && stmt->getNext() != nullptr);
        
        CFGStmt *stmt2 = root;
        do {
            visited[stmt2] = stmt;
            stmt2 = stmt2->getNext();
        } while (stmt2->isA(CFGNopStmt_kind) && stmt2->getNext() != nullptr);
    }
    visited[root] = stmt;
    if (stmt->isA(CFGSimpleStmt_kind)) {
        CFGSimpleStmt *ss = static_cast<CFGSimpleStmt *>(stmt);
        ss->next = CFGRemoveNopsHelper(ss->next, visited);
    } else if (stmt->isA(CFGCBRStmt_kind)) {
        CFGCBRStmt *cbrs = static_cast<CFGCBRStmt *>(stmt);
        cbrs->target_true = CFGRemoveNopsHelper(cbrs->target_true, visited);
        cbrs->target_false = CFGRemoveNopsHelper(cbrs->target_false, visited);
    }
    return stmt;
}

CFGStmt *CFGRemoveNops(CFGStmt *root) 
{
    std::unordered_map<CFGStmt *, CFGStmt *> visited;

    return CFGRemoveNopsHelper(root, visited);
}
