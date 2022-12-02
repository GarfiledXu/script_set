#https://cloud.tencent.com/developer/article/1692550
#shell echo数组


#shell for
local i=0
for item in ${array[@]}
do
    #reference
    $item
    i=$((i+1))
done


#shell 定义空数组


#正确获取数组元素个数
