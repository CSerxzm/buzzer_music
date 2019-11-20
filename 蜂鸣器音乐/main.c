#include <reg52.h>
#define RldTmr(fr) 65536 - (11059200 /12) / (fr << 1)
#define FuDian(n) (n << 1) / 3    //����n�������Ļ���
 
typedef unsigned char UCHAR;
typedef unsigned int UINT;
typedef unsigned long ULONG;
 
sbit BUZZ = P0^0;
 
UINT code noteFreq[] = {    //���� 1-7 �͸��� 1-7��Ӧ��Ƶ���б�
    523, 587, 659, 698, 784, 880, 988,
    1047, 1175, 1319, 1397, 1568, 1760, 1976 ,2,494,
};
UINT code tmrRld[] = {      //���� 1-7 �͸��� 1-7��Ӧ�Ķ�ʱ������ֵ
    RldTmr(523), RldTmr(587), RldTmr(659), RldTmr(698), RldTmr(784), RldTmr(880), RldTmr(988),
    RldTmr(1047), RldTmr(1175), RldTmr(1319), RldTmr(1397), RldTmr(1568), RldTmr(1760), RldTmr(1976),
};
UCHAR code musicNote[] = {      //����
    15,15,15,8,9,
	10,5,8,10,10,15,9,8,
	9,12,12,12,12,15,8,7,
	8,8,8,8,8,15,7,8,
	7,8,7,6,5,15,5,5,
	6,6,6,6,6,15,5,3,
	5,3,5,9,8,15,5,
	10,10,10,11,12,8,8,9,10,
};
UCHAR code noteDuration[] = {       //������Ӧ��ʱֵ��4��ʾ4��������8��ʾ8��������16��ʾ16������
	4,4,4,8,8,
	8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,
	8,8,8,8,4,8,8,
	8,8,8,8,8,8,16,32,32,
};
bit enable = 1, tmrFlg = 0;
UCHAR T0RH = 0XFF, T0RL = 0X00;
 
void Delay(UINT n);
void PlayMusic(UCHAR speed);    //�̶���׼Ϊ4���������ٶȣ�����speed = 108 ��ʾһ����ɨ��108��4������
 
void main() {
    EA = 1;
    TMOD = 0X01;
    TH0 = T0RH;
    TL0 = T0RL;
    ET0 = 1;
    TR0 = 1;
    while (1) {
        PlayMusic(55);
      Delay(40000u);
    }
}
 
void Delay(UINT n) {
    UINT i;
    for (i = 0; i < n; i++) ;
}
 
void PlayMusic(UCHAR speed) {
    UCHAR i;
    UCHAR idx;
    UINT cnt = 0;
    UINT durationCnt = 0;	//��ǰ������ʱֵ��Ӧ�Ķ�ʱ������
    UINT soundCnt = 0;		//��ǰ�����ķ���ʱֵ��Ӧ�ļ���ֵ
    for (i = 0; i < sizeof (musicNote); ) {
        while (!tmrFlg) ;
        tmrFlg = 0;
        if (cnt == 0) {
            idx = musicNote[i] - 1;
            T0RH = tmrRld[idx] >> 8;
            T0RL = tmrRld[idx];
            durationCnt = (ULONG)240 * (ULONG)noteFreq[idx] / ((ULONG)noteDuration[i] * (ULONG)speed);
            soundCnt = durationCnt - (durationCnt >> 2);	//��ǰ����ʱֵ��ǰ3/4��������1/4����
            enable = 1;
            cnt++;
        }
        else {
            if (cnt == durationCnt) {
                cnt = 0;
                i++;
            }
            else {
                cnt++;
                if (cnt == soundCnt) {
                    enable = 0;
                }
            }
        }
    }
}
 
void InterruptTmr0() interrupt 1 {
    TH0 = T0RH;
    TL0 = T0RL;
    tmrFlg = 1;
    if (enable)
        BUZZ = ~BUZZ;
    else
        BUZZ = 1;
}
