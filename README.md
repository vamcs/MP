# MP
Projeto final da disciplina de Métodos de Programação, UnB, 2016/1

# Gitflow Workflow
Resumo:

- Feature (develop -> develop):
<pre>
#Cria o branch feature
git checkout -b myfeature develop
---------------------------------
#Faz merge em develop
git checkout develop
git merge --no-ff myfeature
git branch -d myfeature
git push origin develop
</pre>

- Release (develop -> develop e master):
<pre>
#Cria o branch release
git checkout -b release-1.2 develop
./bump-version.sh 1.2                             #Fictício
git commit -a -m “Bumped version number to 1.2”
---------------------------------
#Faz merge em master
git checkout master
git merge --no-ff release-1.2
git tag -a 1.2
#Faz merge em develop
git checkout develop
git merge --no-ff release-1.2
git branch -d release-1.2
</pre>

- Hotfix (master -> develop e master):
<pre>
#Cria o branch hotfix
git checkout -b hotfix-1.2.1 master
./bump-versio.sh 1.2.1                        #Fictício
git commit -a -m “Bumped version number to 1.2.1”
git commit -m “Fixed severe production problem”
---------------------------------
#Merge em master
git checkout master
    git merge --no-ff hotfix-1.2.1
    git tag -a 1.2.1
#Merge em develop
git checkout develop
    git merge --no-ff hotfix-1.2.1
git branch -d hotfix-1.2.1
</pre>
