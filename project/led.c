#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int

uchar a,count,S1num,xqnum;
char yue,ri,shi,fen,miao;
int nian;

sbit lcdrs=P2^5;
sbit lcdrw=P2^4;
sbit lcden=P2^3;

sbit S1=P3^0;                                //定义键——进入设置
sbit S2=P3^1;                                //定义键——增大
sbit S3=P3^2;                                //定义键——减小
sbit S4=P3^3;                                //定义键——退出设置

uchar code table0[]="2014-08-13 WED";
uchar code table1[]="00:00:00";
uchar code xingqi[][3]={"MON","TUE","WED","THU","FRI","SAT","SUN"};

void delay(uint z)
{
        uint x,y;
        for(x=z;x>0;x--)
                for(y=110;y>0;y--);
}

/**************************************************
1602液晶显示器模块
sbit lcdrs=P2^5;
sbit lcdrw=P2^4;
sbit lcden=P2^3;
*************************************************/
void write_com(uchar com)                                        //液晶写指令
{
        lcdrw=0;
        lcdrs=0;
        P0=com;
        delay(5);
        lcden=1;
        delay(5);
        lcden=0;
}

void write_data(uchar dat)                                        //液晶写数据
{
        lcdrw=0;
        lcdrs=1;
        P0=dat;
        delay(5);
        lcden=1;
        delay(5);
        lcden=0;
} 
/********************************************
年月日写入1602函数
********************************************/
void write_sfm(uchar add,uchar dat)                           //写时分秒函数
{
        uchar shi,ge;
        shi=dat/10;
        ge=dat%10;
        write_com(0x80+0x40+add);
        write_data(0x30+shi);
        write_data(0x30+ge);
}
void write_yr(uchar add,uchar dat)                           //写月日函数
{
        uchar shi,ge;
        shi=dat/10;
        ge=dat%10;
        write_com(0x80+add);
        write_data(0x30+shi);
        write_data(0x30+ge);
}
void write_nian(uchar add,uint dat)                           //写年函数
{
        uint qian,bai,shi,ge;
        qian=dat/1000;
        bai=dat%1000/100;
        shi=dat%100/10;
        ge=dat%10;
        write_com(0x80+add);
        write_data(0x30+qian);
        write_data(0x30+bai);
        write_data(0x30+shi);
        write_data(0x30+ge);
}
/***************************************************
初始化
***************************************************/
void init_lcd()                                                          //液晶初始化
{
        lcden=0;

        nian=2014;
        yue=8;
        ri=13;
        shi=0;                                                                   //初始shi、fen、miao
        fen=0;
        miao=0;

        write_com(0x38);                                           //设置16x2显示，5x7点阵，8位数据口
        write_com(0x0c);                                           //设置开显示，不显示光标
        write_com(0x06);                                           //写一个字符后地址指针加1
        write_com(0x01);                                           //显示清0，数据指针清0
} 

void init()                                                                        //初始化函数
{
        init_lcd();                                                                 //液晶初始化
        write_com(0x80);                                                  //设置显示初始坐标
        for(a=0;a<14;a++)                                                   //显示年月日初始值
        {
                write_data(table0[a]);
                delay(5);
        }
        write_com(0x80+0x40);                                           //设置显示初始坐标
        for(a=0;a<8;a++)                                                        //显示时分秒初始值
        {
                write_data(table1[a]);                                        
                delay(5);
        }

        write_nian(0,nian);
        write_sfm(6,miao);                                                         //分别将shi、fen、miao送去液晶显示
        write_sfm(3,fen);
        write_sfm(0,shi);

        count=0;
        xqnum=0;
        S1num=0;                                                                         //初始化全局变量count

        TMOD=0x01;                                                                          //设置定时器0工作模式1
        TH0=(65535-50000)/256;                                                  //定时器装初始值
        TL0=(65535-50000)%256;
        EA=1;                                                                                        //开总中断
        ET0=1;                                                                                        //开定时器0中断
        TR0=1;                                                                                        //启动定时器0
} 
/**************************************************
独立键盘
sbit S1=P3^0;                                //定义键——进入设置
sbit S2=P3^1;                                //定义键——增大
sbit S3=P3^2;                                //定义键——减小
sbit S4=P3^3;                                //定义键——退出设置
独立键盘已接地
*************************************************/
void keyscan()
{
        if(S1==0)
        {
                delay(5);                                        //确认定义键被按下
                if(S1==0)
                {
                        S1num++;                                //定义键S1按下次数记录
                        while(!S1);                                //释放按键确认
                        if(S1num==1)                        //S1按下一次时
                        {
                                TR0=0;                                //关闭定时器
                                write_com(0xc0+7);        //光标定位到秒位置
                                write_com(0x0f);        //光标闪烁
                        }
                        if(S1num==2)                         //S1按下两次时
                        {
                                write_com(0xc0+4);         //光标定位到分位置
                        }
                        if(S1num==3)                          //S1按下三次时
                        {
                                write_com(0xc0+1);          //光标定位到时位置
                        }
                        if(S1num==4)                          //S1按下四次时
                        {
                                write_com(0x80+13);          //光标定位到星期位置
                        }
                        if(S1num==5)                          //S1按下五次时
                        {
                                write_com(0x80+9);          //光标定位到日位置
                        }
                        if(S1num==6)                          //S1按下六次时
                        {
                                write_com(0x80+6);          //光标定位到月位置
                        }
                        if(S1num==7)                          //S1按下七次时
                        {
                                write_com(0x80+3);          //光标定位到年位置
                        }
                        if(S1num==8)                          //S1按下八次时
                        {
                                S1num=0;                          //S1记录按键次数清零
                                TR0=1;                                  //开启定时器
                                write_com(0x0c);          //取消光标闪烁
                        }
                }
        }
        if(S1num!=0)                                           //只有定义键按下后S2、S3、S4才有效
        {
                if(S2==0)
                {
                        delay(5);                                        //防抖
                        if(S2==0)                                   //确认按键被按下
                        {
                                while(!S2);                                //释放按键确认
                                if(S1num==1)                        //S1按下一次时
                                {
                                        miao++;                                //调整秒加1
                                        if(miao==60)                //满60清零
                                                miao=0;
                                        write_sfm(6,miao);                   //每调节一次送液晶显示一次
                                        write_com(0x80+0x40+6);           //显示位置重新回到调节处
                                }
                                if(S1num==2)                           //S1按下两次时
                                {
                                        fen++;                                        //调整分加1
                                        if(fen==60)
                                                fen=0;
                                        write_sfm(3,fen);
                                        write_com(0x80+0x40+3);
                                }
                                if(S1num==3)                                 //S1按下三次时
                                {
                                        shi++;                                          //调整时加1
                                        if(shi==24)
                                                shi=0;
                                        write_com(0x80+0x40);
                                        write_sfm(0,shi);
                                }
                                if(S1num==4)                                                   //星期加调整
                                {
                                        xqnum++;
                                        if(xqnum==7)
                                                xqnum=0;
                                        write_com(0x80+0x0b);
                                        for(a=0;a<3;a++)
                                        {
                                                write_data(xingqi[xqnum][a]);
                                                delay(5);
                                        }
                                }
                                if(S1num==5)                                                                //日加调整
                                {
                                        ri++;
                                        if(yue==2)
                                        {
                                                if(nian%400==0)
                                                {
                                                        if(ri==30)
                                                        {
                                                                ri=1;
                                                        }
                                                }
                                                if(nian%400!=0)
                                                {
                                                        if(ri==29)
                                                        {
                                                                ri=1;
                                                        }
                                                }
                                        }
                                        else if(yue<=7)
                                        {
                                                if(yue%2==0&yue!=2)
                                                {
                                                        if(ri==31)
                                                        {
                                                                ri=1;
                                                        }
                                                }
                                                else if(yue%2!=0&yue!=2)
                                                {
                                                        if(ri==32)
                                                        {
                                                                ri=1;
                                                        }
                                                }
                                        }
                                        else if(yue>=8)
                                        {
                                                if(yue%2==0)
                                                {
                                                        if(ri==32)
                                                        {
                                                                ri=1;
                                                        }
                                                }
                                                else if(yue%2!=0)
                                                {
                                                        if(ri==31)
                                                        {
                                                                ri=1;
                                                        }
                                                }
                                        }
                                        write_yr(8,ri);
                                }
                                if(S1num==6)                                                        //月加调整
                                {
                                        yue++;
                                        if(yue==13)
                                                yue=1;
                                        write_yr(5,yue);
                                }                                                                                 
                                if(S1num==7)                                                 //年加调整
                                {
                                        nian++;
                                        if(nian==2019)
                                                nian=2014;
                                        write_nian(0,nian);
                                }
                        }
                }
                if(S3==0)
                {
                        delay(5);
                        if(S3==0)                                                 //确认按键被按下
                        {
                                while(!S3);
                                if(S1num==1)                                 
                                {
                                        miao--;                                          //调整秒减1
                                        if(miao==-1)                          //减到00后再减重新设置为59
                                                miao=59;
                                        write_sfm(6,miao);
                                        write_com(0x80+0x40+6);
                                }
                                if(S1num==2)
                                {
                                        fen--;                                           //调整分减1
                                        if(fen==-1)
                                                fen=59;
                                        write_sfm(3,fen);
                                        write_com(0x80+0x40+3);
                                }
                                if(S1num==3)
                                {
                                        shi--;                                                //调整时减1
                                        if(shi==-1)
                                                shi=23;
                                        write_sfm(0,shi);
                                        write_com(0x80+0x40);
                                }
                                if(S1num==4)
                                {
                                        xqnum--;                                        //调整星期减一
                                        if(xqnum==-1)
                                                xqnum=6;
                                        write_com(0x80+0x0b);
                                        for(a=0;a<3;a++)
                                        {
                                                write_data(xingqi[xqnum][a]);
                                                delay(5);
                                        }
                                }
                                if(S1num==5)                                   //调整日
                                {
                                        ri--;                                                
                                        if(yue==2)
                                        {
                                                if(nian%400==0)
                                                {
                                                        if(ri==0)
                                                        {
                                                                ri=29;
                                                        }
                                                }
                                                if(nian%400!=0)
                                                {
                                                        if(ri==0)
                                                        {
                                                                ri=28;
                                                        }
                                                }
                                        }
                                        else if(yue<=7)                                  
                                        {
                                                if(yue%2==0&yue!=2)
                                                {
                                                        if(ri==0)
                                                        {
                                                                ri=30;
                                                        }
                                                }
                                                else if(yue%2!=0&yue!=2)
                                                {
                                                        if(ri==0)
                                                        {
                                                                ri=31;
                                                        }
                                                }
                                        }
                                        else if(yue>=8)
                                        {
                                                if(yue%2==0)
                                                {
                                                        if(ri==0)
                                                        {
                                                                ri=31;
                                                        }
                                                }
                                                else if(yue%2!=0)
                                                {
                                                        if(ri==0)
                                                        {
                                                                ri=30;
                                                        }
                                                }
                                        }
                                        write_yr(8,ri);                                                 
                                }
                                if(S1num==6)                                           //调整月
                                {
                                        yue--;
                                        if(yue==0)
                                                yue=12;
                                        write_yr(5,yue);
                                }
                                if(S1num==7)                                          //调整年
                                {
                                        nian--;
                                        if(nian==2013)
                                                nian=2018;
                                        write_nian(0,nian);
                                }
                        }
                }
                if(S4==0)                                                                   //退出设置、开启中断
                {
                        delay(5);
                        if(S4==0)
                        {
                                S1num=0;
                                TR0=1;
                                write_com(0x0c);
                        }
                }
        }
}

/**************************************************
定时器0
*************************************************/
void timer0() interrupt 1                                         //定时器0中断服务程序
{
        TH0=(65535-50000)/256;                                         //重装定时器初始值
        TL0=(65535-50000)%256;
        count++;                                                                  //中断次数累加
        if(count==20)                                                         //20次50毫秒即一秒
        {
                count=0;
                miao++;
                if(miao==60)                                                  //秒加到60时分进位
                {
                        miao=0;
                        fen++;
                        if(fen==60)                                                  //分加到60时时进位
                        {
                                fen=0;
                                shi++;
                                if(shi==24)                                                //时加到24时清0
                                {
                                        shi=0;
                                        xqnum++;
                                        ri++;

                                        if(yue==2)                                        //如果是二月
                                        {
                                                if(nian%400==0)                         //闰年判断
                                                {
                                                        if(ri==30)
                                                        {
                                                                ri=1;
                                                                yue++;
                                                                write_yr(5,yue);
                                                        }
                                                }
                                                if(nian%400!=0)                           //平年判断
                                                {
                                                        if(ri==29)
                                                        {
                                                                ri=1;
                                                                yue++;
                                                                write_yr(5,yue);
                                                        }
                                                }
                                        }
                                        else if(yue<=7&yue!=2)           //一月到七月 
                                        {
                                                if(yue%2==0)                //偶数月（除二月）
                                                {
                                                        if(ri==31)                                          
                                                        {
                                                                ri=1;
                                                                yue++;
                                                        }
                                                }
                                                else if(yue%2!=0)                         //奇数月
                                                {
                                                        if(ri==32)
                                                        {
                                                                ri=1;
                                                                yue++;
                                                        }
                                                }
                                                write_yr(5,yue);
                                        }
                                        else if(yue>=8)                                         //八月到12月
                                        {
                                                if(yue%2==0)                //偶数月（除二月）
                                                {
                                                        if(ri==32)                                          
                                                        {
                                                                ri=1;
                                                                yue++;
                                                                if(yue==13)                                   //如果判断月份为12月，则加一后重新定义月份为1
                                                                {
                                                                        yue=1;
                                                                        nian++;
                                                                        write_nian(0,nian);
                                                                }
                                                                write_yr(5,yue);
                                                        }
                                                }
                                                else if(yue%2!=0)                         //奇数月
                                                {
                                                        if(ri==31)
                                                        {
                                                                ri=1;
                                                                yue++;
                                                                write_yr(5,yue);
                                                        }
                                                }
                                        }
                                        write_yr(8,ri);

                                        if(xqnum==7)                                        //星期写入
                                                xqnum=0;
                                        write_com(0x80+0x0b);
                                        for(a=0;a<3;a++)
                                        {
                                                write_data(xingqi[xqnum][a]);
                                                delay(5);
                                        }
                                }
                                write_sfm(0,shi);                                //重新写入数据
                        }
                        write_sfm(3,fen);                                        //重新写入数据
                }
                write_sfm(6,miao);                                                //重新写入数据
        }
}
/*************************************************
主函数
*************************************************/
void main()
{
        init();
        while(1)
        {
                keyscan();                                                                //不停的检测按键是否被按下
        }
}