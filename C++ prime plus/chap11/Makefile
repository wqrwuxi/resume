TAR = work
#下面两个o有顺序区别
OBJ = stonewt.o review_11_1.o
#OBJ = $(wildward *.o)错

#如果不考虑work生成后clean，all去掉也没关系
#想要work生成后clean，就按下面做
#其实all是个隐式的伪目标
all: $(TAR) clean

$(TAR):$(OBJ)
	$(CXX) -o $(TAR) $(OBJ)

$(OBJ): %.o: %.cpp
	$(CXX) -o $@ -c $(CXXFLAGS) $<

#.PHONY:clean cleanall 这句有没有都可以
clean:
	$(RM) $(OBJ)

cleanall:
	rm -rf $(OBJ) $(TAR)
