## git pull exist conflict, to show by git status

## git stash 

## 当存在冲突，调用merge进行直接合并时，是先合并无冲突部分，再报错conflict部分, 即merge中止状态

## 使用merge --abort 退回中止前状态


## 使用mergetool 进行合并编辑 解决文件冲突，再合并，再merge, 再push，但发生了This commit does not belong to any branch on this repository, and may belong to a fork outside of the repository. 

## specification and workflow 
## 为了确保无障碍的在多个本地端进行push和pull，要确保到有修改即push，让本地不存在历史版本被修改即无远程冲突，即可保持进行pull操作的场景永远是历史commit拉去高版本remote commit