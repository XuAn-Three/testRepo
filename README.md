### git操作

![image-20221202165646995](C:\Users\W3Cschool\AppData\Roaming\Typora\typora-user-images\image-20221202165646995.png)

#### 在命令行创建一个新的仓库

```
git init
git add <file>[所有文件 .]
git commit -m “注释”
git branch -M main
git remote add origin git@github.com:...
git push -u origin main
```

#### 在命令行上传已存在的仓库

```
git remote add origin git@github.com:...
git branch -M main
git push -u origin main
```

#### 从其他仓库导入代码



`git init`

用于在当前目录创建新的 git 仓库。

使用该命令后，当前目录中将会创建一个 `.git` 的子目录，这就是你的 git 仓库。

所有有关项目的快照数据都存放在这里。

`.git` 目录默认是隐藏的。



`git add`

用来将文件添加到暂存区。

一次性可以添加一个或者多个文件夹。

`git add <file1> <file2>`

添加当前目录下的所有文件到暂存区。

`git add .`



`git status`

查看在你上次提交之后，是否有对文件进行再修改。

可以通过参数 `-s` 获取简单输出。



`git commit`

该命令是用于将暂存区里面的内容提交到仓库。

`-m` 参数后面加上修改信息注释。

`-a` 参数设置修改文件后不需要执行git add 命令，直接提交到仓库。

在提交之前，需要先设置提交的用户信息：用户名和邮箱

`git config --global user.name "username"`

`git config --global user.email "useremail"`