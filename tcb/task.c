#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// スイッチのON/OFFを1/0で保持するグローバル変数
int SW_STATUS;

void led_ctrl(void){
    /*
    スイッチのON/OFFに合わせてLEDを制御する関数
    ON: LEDを点灯
    OFF: 徐々に消灯
    スイッチとLEDが1つずつ, 1対1に対応していると想定
    -----
    以下のコメントは次のような意味を持つとする
    - 「GPIO = LED_ON」：GPOIに接続されたLEDに対して点灯命令を出す
    - 「GPIO = LED_OFF」：GPOIに接続されたLEDに対して消灯命令を出す
    - 「TSK_SLEEP(mSec)」：mSecだけタスクを休眠状態にするシステムコール
    */
    // PWM制御用
    int pwm_width;
    // 関数呼び出し前のスイッチのON/OFFを保持する
    static int before_SW;

    if(SW_STATUS == 1){
        // スイッチがON
        printf("LED: ON\n");  // GPIO_1 = LED_ON
        before_SW = 1;
    }
    else{
        // スイッチがOFF
        if(before_SW == 1){
            // スイッチがON→OFFになった場合のみ
            for(pwm_width = 0; pwm_width < 3; pwm_width++){
                printf("LED: ON\n");  // GPIO_1 = LED_ON
                sleep(3 - pwm_width);  // TSK_SLEEP(3 - pwm_width)
                printf("LED: OFF\n");  // GPIO_1 = LED_OFF
                sleep(pwm_width);  // // TSK_SLEEP(pwm_width)
            }
        }
        before_SW = 0;
        printf("LED: OFF\n");  // // GPIO_1 = LED_OFF
    }

}

void sw_check(void){
    /*
    スイッチの状態を確認する関数
    ON→1, OFF→0
    */
    srand((unsigned int)time(NULL));
    int num;
    num = rand()%9;
    if(num >= 4){
        SW_STATUS = 1;
        printf("SW: ON\n");
    }
    else{
        SW_STATUS = 0;
        printf("SW: OFF\n");
    }
    // rand()対策
    sleep(1);
}