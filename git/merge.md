## git pull exist conflict, to show by git status

## git stash 

## 当存在冲突，调用merge进行直接合并时，是先合并无冲突部分，再报错conflict部分, 即merge中止状态

## 使用merge --abort 退回中止前状态


## 使用mergetool 进行合并编辑 解决文件冲突，再合并，再merge, 再push，但发生了This commit does not belong to any branch on this repository, and may belong to a fork outside of the repository. 

## specification and workflow 
## 为了确保无障碍的在多个本地端进行push和pull，要确保到有修改即push，让本地不存在历史版本被修改即无远程冲突，即可保持进行pull操作的场景永远是历史commit拉去高版本remote commit
### remote最新，local端的old commit 有改动，这个时候需要先形成commit，再解决冲突，因为merge pull的对象都是提交，而不是workspace,所为冲突解决，就是修改文件，达到若再产生的提交可以自然合并为一个分支
### local 冲突 -> 生成冲突提交(即将改动的部分生成commit) -> 与远程提交间的冲突处理, merge + mergetool + 文件修改 + 冲突处理的目的只有一个，文件改好，再生成一个没有冲突的commit ->此时已经有两个commit且无提交，那么可以merge 生成一个本地的合并的commit ->总结就是获得冲突commit后不停修改文件生成commit，最后获得无冲突的commit