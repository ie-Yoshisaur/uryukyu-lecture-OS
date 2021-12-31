echo 'ssh to:'
read server

directories=($(ssh ${server} "ls ~"))

for ((i=0;i<${#directories[@]};i++))
do
ssh ${server} "find ~/${directories[i]} -type f -exec file {} \;"
done
