[](oftenstart)
[](include:yes)
[](name:root)
[](type:class)
[](superclass:)
[](oftenstop)

# OFTen

## Why OFTen?
* Open [](composition,open,composed_by) test
* Free [](composition,free,composed_by) test 
* Tidy [](composition,tidy,composed_by) test

are the basic requirements for the evolution of knowledge.

OFTen is a tool developed to make it easier to write papers.
OFTen allows you to dynamically generate UML diagrams by interpreting tags present in the text according to the markdown standard.
It is possible to represent the relationships between the various topics covered in the paper through the aggregation, composition and extension relationships provided for by the UML standard. The result will be a more understandable and intuitive paper to study.

## What you can do with OFTen?
Consider that _paper/_ folder contains all your markdown (*.md) files for each topic covered in the paper.

---
Choose the input folder and produce an UML diagram.
```
./often -i folder/
```
option _-s_ is by default.
```
./often -i folder/ -s
```
---
Choose the input folder and produce real time UML diagram. This will continue running until ^C.
```
./often -i folder/ -p
```
---
Produce a single TeX pdf with all markdown files
```
./often -i folder/ -single
```
---
Produce a single TeX pdf for every markdown file
```
./often -i folder/ -multiple
```

## What OFTen will do?
* [x] uml generation
* [x] markdown interpretation
* [x] uml class diagram of paper
* [x] dynamic uml class diagram
* [ ] multi-file management
* [ ] elegant markdown to latek
* [ ] integration with mathematica
* [ ] integration with pandoc
* [ ] multi platform

## Build OFTen
```
sudo apt install -y texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
sudo apt install -y pandoc graphviz
sudo apt install -y default-jre
wget https://github.com/plantuml/plantuml/releases/download/v1.2021.16/plantuml-1.2021.16.jar
g++ main.cpp -o often -O3 -std=c++17
```
