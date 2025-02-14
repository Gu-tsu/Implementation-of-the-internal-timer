#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int

uchar a,count,S1num,xqnum;
char yue,ri,shi,fen,miao;
int nian;

sbit lcdrs=P2^5;
sbit lcdrw=P2^4;
sbit lcden=P2^3;

sbit S1=P3^0;                                //�����������������
sbit S2=P3^1;                                //�������������
sbit S3=P3^2;                                //�����������С
sbit S4=P3^3;                                //����������˳�����

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
1602Һ����ʾ��ģ��
sbit lcdrs=P2^5;
sbit lcdrw=P2^4;
sbit lcden=P2^3;
*************************************************/
void write_com(uchar com)                                        //Һ��дָ��
{
        lcdrw=0;
        lcdrs=0;
        P0=com;
        delay(5);
        lcden=1;
        delay(5);
        lcden=0;
}

void write_data(uchar dat)                                        //Һ��д����
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
������д��1602����
********************************************/
void write_sfm(uchar add,uchar dat)                           //дʱ���뺯��
{
        uchar shi,ge;
        shi=dat/10;
        ge=dat%10;
        write_com(0x80+0x40+add);
        write_data(0x30+shi);
        write_data(0x30+ge);
}
void write_yr(uchar add,uchar dat)                           //д���պ���
{
        uchar shi,ge;
        shi=dat/10;
        ge=dat%10;
        write_com(0x80+add);
        write_data(0x30+shi);
        write_data(0x30+ge);
}
void write_nian(uchar add,uint dat)                           //д�꺯��
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
��ʼ��
***************************************************/
void init_lcd()                                                          //Һ����ʼ��
{
        lcden=0;

        nian=2014;
        yue=8;
        ri=13;
        shi=0;                                                                   //��ʼshi��fen��miao
        fen=0;
        miao=0;

        write_com(0x38);                                           //����16x2��ʾ��5x7����8λ���ݿ�
        write_com(0x0c);                                           //���ÿ���ʾ������ʾ���
        write_com(0x06);                                           //дһ���ַ����ַָ���1
        write_com(0x01);                                           //��ʾ��0������ָ����0
} 

void init()                                                                        //��ʼ������
{
        init_lcd();                                                                 //Һ����ʼ��
        write_com(0x80);                                                  //������ʾ��ʼ����
        for(a=0;a<14;a++)                                                   //��ʾ�����ճ�ʼֵ
        {
                write_data(table0[a]);
                delay(5);
        }
        write_com(0x80+0x40);                                           //������ʾ��ʼ����
        for(a=0;a<8;a++)                                                        //��ʾʱ�����ʼֵ
        {
                write_data(table1[a]);                                        
                delay(5);
        }

        write_nian(0,nian);
        write_sfm(6,miao);                                                         //�ֱ�shi��fen��miao��ȥҺ����ʾ
        write_sfm(3,fen);
        write_sfm(0,shi);

        count=0;
        xqnum=0;
        S1num=0;                                                                         //��ʼ��ȫ�ֱ���count

        TMOD=0x01;                                                                          //���ö�ʱ��0����ģʽ1
        TH0=(65535-50000)/256;                                                  //��ʱ��װ��ʼֵ
        TL0=(65535-50000)%256;
        EA=1;                                                                                        //�����ж�
        ET0=1;                                                                                        //����ʱ��0�ж�
        TR0=1;                                                                                        //������ʱ��0
} 
/**************************************************
��������
sbit S1=P3^0;                                //�����������������
sbit S2=P3^1;                                //�������������
sbit S3=P3^2;                                //�����������С
sbit S4=P3^3;                                //����������˳�����
���������ѽӵ�
*************************************************/
void keyscan()
{
        if(S1==0)
        {
                delay(5);                                        //ȷ�϶����������
                if(S1==0)
                {
                        S1num++;                                //�����S1���´�����¼
                        while(!S1);                                //�ͷŰ���ȷ��
                        if(S1num==1)                        //S1����һ��ʱ
                        {
                                TR0=0;                                //�رն�ʱ��
                                write_com(0xc0+7);        //��궨λ����λ��
                                write_com(0x0f);        //�����˸
                        }
                        if(S1num==2)                         //S1��������ʱ
                        {
                                write_com(0xc0+4);         //��궨λ����λ��
                        }
                        if(S1num==3)                          //S1��������ʱ
                        {
                                write_com(0xc0+1);          //��궨λ��ʱλ��
                        }
                        if(S1num==4)                          //S1�����Ĵ�ʱ
                        {
                                write_com(0x80+13);          //��궨λ������λ��
                        }
                        if(S1num==5)                          //S1�������ʱ
                        {
                                write_com(0x80+9);          //��궨λ����λ��
                        }
                        if(S1num==6)                          //S1��������ʱ
                        {
                                write_com(0x80+6);          //��궨λ����λ��
                        }
                        if(S1num==7)                          //S1�����ߴ�ʱ
                        {
                                write_com(0x80+3);          //��궨λ����λ��
                        }
                        if(S1num==8)                          //S1���°˴�ʱ
                        {
                                S1num=0;                          //S1��¼������������
                                TR0=1;                                  //������ʱ��
                                write_com(0x0c);          //ȡ�������˸
                        }
                }
        }
        if(S1num!=0)                                           //ֻ�ж�������º�S2��S3��S4����Ч
        {
                if(S2==0)
                {
                        delay(5);                                        //����
                        if(S2==0)                                   //ȷ�ϰ���������
                        {
                                while(!S2);                                //�ͷŰ���ȷ��
                                if(S1num==1)                        //S1����һ��ʱ
                                {
                                        miao++;                                //�������1
                                        if(miao==60)                //��60����
                                                miao=0;
                                        write_sfm(6,miao);                   //ÿ����һ����Һ����ʾһ��
                                        write_com(0x80+0x40+6);           //��ʾλ�����»ص����ڴ�
                                }
                                if(S1num==2)                           //S1��������ʱ
                                {
                                        fen++;                                        //�����ּ�1
                                        if(fen==60)
                                                fen=0;
                                        write_sfm(3,fen);
                                        write_com(0x80+0x40+3);
                                }
                                if(S1num==3)                                 //S1��������ʱ
                                {
                                        shi++;                                          //����ʱ��1
                                        if(shi==24)
                                                shi=0;
                                        write_com(0x80+0x40);
                                        write_sfm(0,shi);
                                }
                                if(S1num==4)                                                   //���ڼӵ���
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
                                if(S1num==5)                                                                //�ռӵ���
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
                                if(S1num==6)                                                        //�¼ӵ���
                                {
                                        yue++;
                                        if(yue==13)
                                                yue=1;
                                        write_yr(5,yue);
                                }                                                                                 
                                if(S1num==7)                                                 //��ӵ���
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
                        if(S3==0)                                                 //ȷ�ϰ���������
                        {
                                while(!S3);
                                if(S1num==1)                                 
                                {
                                        miao--;                                          //�������1
                                        if(miao==-1)                          //����00���ټ���������Ϊ59
                                                miao=59;
                                        write_sfm(6,miao);
                                        write_com(0x80+0x40+6);
                                }
                                if(S1num==2)
                                {
                                        fen--;                                           //�����ּ�1
                                        if(fen==-1)
                                                fen=59;
                                        write_sfm(3,fen);
                                        write_com(0x80+0x40+3);
                                }
                                if(S1num==3)
                                {
                                        shi--;                                                //����ʱ��1
                                        if(shi==-1)
                                                shi=23;
                                        write_sfm(0,shi);
                                        write_com(0x80+0x40);
                                }
                                if(S1num==4)
                                {
                                        xqnum--;                                        //�������ڼ�һ
                                        if(xqnum==-1)
                                                xqnum=6;
                                        write_com(0x80+0x0b);
                                        for(a=0;a<3;a++)
                                        {
                                                write_data(xingqi[xqnum][a]);
                                                delay(5);
                                        }
                                }
                                if(S1num==5)                                   //������
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
                                if(S1num==6)                                           //������
                                {
                                        yue--;
                                        if(yue==0)
                                                yue=12;
                                        write_yr(5,yue);
                                }
                                if(S1num==7)                                          //������
                                {
                                        nian--;
                                        if(nian==2013)
                                                nian=2018;
                                        write_nian(0,nian);
                                }
                        }
                }
                if(S4==0)                                                                   //�˳����á������ж�
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
��ʱ��0
*************************************************/
void timer0() interrupt 1                                         //��ʱ��0�жϷ������
{
        TH0=(65535-50000)/256;                                         //��װ��ʱ����ʼֵ
        TL0=(65535-50000)%256;
        count++;                                                                  //�жϴ����ۼ�
        if(count==20)                                                         //20��50���뼴һ��
        {
                count=0;
                miao++;
                if(miao==60)                                                  //��ӵ�60ʱ�ֽ�λ
                {
                        miao=0;
                        fen++;
                        if(fen==60)                                                  //�ּӵ�60ʱʱ��λ
                        {
                                fen=0;
                                shi++;
                                if(shi==24)                                                //ʱ�ӵ�24ʱ��0
                                {
                                        shi=0;
                                        xqnum++;
                                        ri++;

                                        if(yue==2)                                        //����Ƕ���
                                        {
                                                if(nian%400==0)                         //�����ж�
                                                {
                                                        if(ri==30)
                                                        {
                                                                ri=1;
                                                                yue++;
                                                                write_yr(5,yue);
                                                        }
                                                }
                                                if(nian%400!=0)                           //ƽ���ж�
                                                {
                                                        if(ri==29)
                                                        {
                                                                ri=1;
                                                                yue++;
                                                                write_yr(5,yue);
                                                        }
                                                }
                                        }
                                        else if(yue<=7&yue!=2)           //һ�µ����� 
                                        {
                                                if(yue%2==0)                //ż���£������£�
                                                {
                                                        if(ri==31)                                          
                                                        {
                                                                ri=1;
                                                                yue++;
                                                        }
                                                }
                                                else if(yue%2!=0)                         //������
                                                {
                                                        if(ri==32)
                                                        {
                                                                ri=1;
                                                                yue++;
                                                        }
                                                }
                                                write_yr(5,yue);
                                        }
                                        else if(yue>=8)                                         //���µ�12��
                                        {
                                                if(yue%2==0)                //ż���£������£�
                                                {
                                                        if(ri==32)                                          
                                                        {
                                                                ri=1;
                                                                yue++;
                                                                if(yue==13)                                   //����ж��·�Ϊ12�£����һ�����¶����·�Ϊ1
                                                                {
                                                                        yue=1;
                                                                        nian++;
                                                                        write_nian(0,nian);
                                                                }
                                                                write_yr(5,yue);
                                                        }
                                                }
                                                else if(yue%2!=0)                         //������
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

                                        if(xqnum==7)                                        //����д��
                                                xqnum=0;
                                        write_com(0x80+0x0b);
                                        for(a=0;a<3;a++)
                                        {
                                                write_data(xingqi[xqnum][a]);
                                                delay(5);
                                        }
                                }
                                write_sfm(0,shi);                                //����д������
                        }
                        write_sfm(3,fen);                                        //����д������
                }
                write_sfm(6,miao);                                                //����д������
        }
}
/*************************************************
������
*************************************************/
void main()
{
        init();
        while(1)
        {
                keyscan();                                                                //��ͣ�ļ�ⰴ���Ƿ񱻰���
        }
}