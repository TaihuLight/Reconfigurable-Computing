%PLA算法,以二维数据为例%
clear; clf; clc;

%训练数据
x=[1 4 5 2 7 4 8 9 3 6; 10 5 7 3 5 6 3 10 8 2;-1 -1 -1 -1 1 -1 1 1 -1 1];  %三维数组，最后一维是label
 ind=randperm(10);
 x=x(:,ind);

%数据散点图预览
figure(1)
for k=1:size(x,2)   %按列循环
    if x(3,k)==-1
        plot(x(1,k),x(2,k),'*r','markersize',10);  %label为-1的点,标记号为*
        hold on
    else
         plot(x(1,k),x(2,k),'ob','markersize',10); %lecel为1的点，标记号为o
        hold on
    end
end
axis([0,12,0,15])       %横竖坐标的范围

%开始算法
w0=0;
x0=1;
W=[w0,1,0];              %初始权重值[w0,w1,w2],维度d=2，0向量
inter=true;             %初始化迭代条件
inter_num=0;            %迭代次数
while inter==true
    inter_num=inter_num+1;  %计算迭代次数
    error=0;                %初始化分类错误的数量
    for j=1:size(x,2)       %循环遍历所有点 
        sign_v=sign(W(1)*x0+W(:,2:3)*x(1:2,j));  %计算分类的符号，默认W(1)=0,x0=1       
        if sign_v~=x(3,j)                       %分类符号与label不一致的情况下，对权重W进行修正         
            W(1)=W(1)+x(3,j)*1;                 %默认x0=1，更新W(1)的值
            W(:,2:3)=W(:,2:3)+x(3,j)*x(1:2,j)'; %根据w'=w+y*x更新W(2)和W(3)的值
            error = error+1;                    %存在分类错误的点，error+1
        end
    end
    if error<=1
        inter=false;    %error=0的情况下，即全部分类正确，则退出迭代
    end
end

inter_num         %输出迭代字数
W                %输出权重的最后结果

%在散点图中画出分类直线
x1=[1 3 5 7 9 10 11];       %直线横坐标值
y1=-W(2)/W(3)*x1-W(1)/W(3); %直线纵坐标值，由W(1)+W(2)x1+W(3)y1=0得到
plot(x1,y1,'-g');           %画出分类直线
