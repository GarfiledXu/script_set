https://cloud.tencent.com/developer/article/1430091

https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E6%89%93%E6%A0%87%E7%AD%BE

## 创建
git tag \<tag>

git tag -a \<tag> -m \<message>

## 删除
git tag -d \<tag>

## 查询信息
git tag

git show \<tag>

git 可视tag为独立的指针信息，与commit关联，但保持独立，push commit时并不会附带tag信息，tag信息需要单独push
## 更新远程tag信息
git push \<remote_name e.i:origin> <tag>

git push \<remote_name e.i:origin> --tags


#git commit本身的信息标识比较抽象，在进行版本切换时，tag标签的内容显得格外重要，所以tag的生成 管理 查询 以及关联操作需要牢记

#？git如何拉取指定分支的指定tag
#？不同分支以及不同分支的tag关系
#？tag的信息查询
#？tag的其他关联操作


## 通过tag切换版本
#？ git reset \<tag>

## 以某个tag为基础，创建分支
$ git checkout -b [branch name] [tag name]
