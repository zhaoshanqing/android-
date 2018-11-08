/*
识别关键词的个数为SUM；
n为数组中对应关键词的序列号，例如数组sRecog中的第一个关键词为“xiao jian”则对应的序列号为0；
Voice.Initialise();初始化设置输入方式MIC/MONO。
*/
#include <TimerOne.h>
#include <avr/wdt.h>　　//看门狗
#include <Voice.h>
#include <Syn6288.h>

typedef Syn6288 syn;
uint8  nAsrStatus=0;
#define SUM 11

#define light_sensor_pin A2  //光线传感器插模拟口2
#define light_threshold 200  //光线传感器阈值
#define relay 8              //继电器插数字口8
#define state_led 7          //状态指示灯插数字口7

char sRecog[SUM][30] = {"xiao jian guo"," ","kai deng","guan deng","xu yao kai deng ma","xiao jian","xiao","jian guo"," ","kai","guan"};
//小坚果，开灯，关灯，需要开灯吗


uint8_t text2[]={0xC4,0xFA,0xBA,0xC3};// 您好

uint8_t text8[]={0xD5,0xFD,0xD4,0xDA,0xBF,0xAA,0xB5,0xC6};//正在开灯
uint8_t text9[]={0xD5,0xFD,0xD4,0xDA,0xB9,0xD8,0xB5,0xC6};//正在关灯
uint8_t text10[]={0xC7,0xEB,0xCE,0xCA,0xC4,0xE3,0xD4,0xDA,0xCB,0xB5,0xCA,0xB2,0xC3,0xB4,0xA3,0xBF};//请问你在说什么？
uint8_t text11[]={0xB9,0xE2,0xCF,0xDF,0xB2,0xBB,0xD7,0xE3,0x20,0xA3,0xAC,0xD5,0xFD,0xD4,0xDA,0xBF,0xAA,0xB5,0xC6};//光线不足，正在开灯
uint8_t text12[]={0xCF,0xD6,0xD4,0xDA,0xB9,0xE2,0xCF,0xDF,0xC3,0xF7,0xC1,0xC1,0xA3,0xAC,0xB2,0xBB,0xD0,0xE8,0xD2,0xAA,0xBF,0xAA,0xB5,0xC6};//光线明亮，不需要开灯

uint8_t flag;  //标志位，收到小坚果口令后将置为1，动作执行完后清零

void finally(uint8_t n) //n为sRecog数组中对应关键词的序列号
{
  switch(n)
  {
        case 0: syn.play(text2,sizeof(text2),0);flag=1;break; // 您好
  
         
    case 2: //开灯
             if(flag==1)
             {
               syn.play(text8,sizeof(text8),0); //播放正在开灯
               digitalWrite(relay,HIGH);  //高电平驱动
               flag=0;  
             }
      break;  
        case 3: //关灯
       if(flag==1)
       {
        syn.play(text9,sizeof(text9),0); //播放正在关灯
        digitalWrite(relay,LOW);
        flag=0;
      }
      break;
        case 4: //需要开灯吗？
      if(flag==1)
       {
        int shine=analogRead(light_sensor_pin); //读取光照度模拟量
        //Serial.print("Light:");
        //Serial.println(shine);
        if(shine < light_threshold) //小于光照度阀值时
        {
          syn.play(text11,sizeof(text11),0); //光线不足，正在开灯
          digitalWrite(relay,HIGH);
        }
        else 
        {
          syn.play(text12,sizeof(text12),0); //光线明亮，不需要开灯syn.play(指针，数组长度，music)
          digitalWrite(relay,LOW);
        }
        flag=0;
       }
            break;
    default:syn.play(text10,sizeof(text10),0);break;  //非正常命令，请问你在说什么     
  }
}

void ExtInt0Handler ()
{
  Voice.ProcessInt0();  //芯片送出中断信号
        
}
void timerIsr()
{
  wdt_reset(); //为了避免板在正常操作中重启，wdt_reset()定期调用命令以重置监视程序
}
void setup()
{
  Serial.begin(9600);　　//设置波特率
  Voice.Initialise(MIC,VoiceRecognitionV1);//初始化模式MIC 或者　MONO,默认是MIC 
                                           //VoiceRecognitionV1 is VoiceRecognitionV1.0 shield
                                           //VoiceRecognitionV2 is VoiceRecognitionV2.1 module  
  attachInterrupt(0,ExtInt0Handler,LOW);
 
  pinMode(relay,OUTPUT);  //设置继电器插口为输出引脚
  digitalWrite(relay,LOW);//继电器插口输出为低电平
  pinMode(state_led,OUTPUT);//状态测试灯插口为输出引脚
  digitalWrite(state_led,LOW);//状态测试灯输出高电平
  Timer1.initialize(90000);　　//设置周期
  Timer1.attachInterrupt(timerIsr); // attach the service routine here
  wdt_enable(WDTO_120MS);　　//启用看门狗
}
void loop()
{  
    static uint8_t nAsrRes=0;　　//定义变量用于保存返回值
    static unsigned long time = millis();  //获取当前时间
  if(millis()-time>100)　　//用最近时刻减去上一时段的时刻
  {  
     
    switch(nAsrStatus)　　//根据nAsrStatus的取值，执行相应的操作
    {
      case LD_ASR_RUNING:　//表示LD3320正在做ASR识别
      case LD_ASR_ERROR:break;//表示一次识别流程中，LD3320芯片内部出现不正确的状态
      case LD_ASR_NONE://表示没有在做ASR识别
      {
        nAsrStatus=LD_ASR_RUNING;
                if (Voice.RunASR(SUM,80,sRecog)==0) //识别不正确
        {
          nAsrStatus= LD_ASR_ERROR;　//表示LD3320内部出现错误
                    Serial.println( "ASR_ERROR"); 
        }

                Serial.println( "ASR_RUNING.....");
        digitalWrite(state_led,HIGH);
        break;
      }
      case LD_ASR_FOUNDOK:　//表示一次识别流程结束后，有一个识别结果
      {
        digitalWrite(state_led,LOW);//状态指示灯灭
        nAsrRes =Voice. LD_GetResult();// 一次ASR识别流程结束，去取ASR识别结果            
                finally(nAsrRes);//调用finally(uint8_t n)函数
        nAsrStatus = LD_ASR_NONE;//执行结束，改变nAsrStatus的值，说明当前没有在做ASR识别
        break;
      }
      case LD_ASR_FOUNDZERO://表示一识别结束后，没有识别结果
      default://其他情况
      { 
        digitalWrite(state_led,LOW);//状态指示灯灭
        nAsrStatus = LD_ASR_NONE;
        break;
      }
      }// switch

  }//if
}