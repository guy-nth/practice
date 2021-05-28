/*
    ## 前提知識
    - ファイルディスクリプタ(File Descriptor, FD)
        プログラムがアクセスするファイルや入出力先を識別するための番号(ID)
        また、紐付いているファイルや入出力先の情報も合わせて持っている
        通常、FDは0から順に割り振られ、一般的にプログラムが実行されている間は
        - 0: 標準入力(stdin)
        - 1: 標準出力(stdout)
        - 2: 標準エラー出力(stderr)
        が常に使用中になっているので、新しくFDが作られた場合は3から割り振られることになる
        FDを破棄すると割り当てられた番号は未使用状態になる
        例)
        FD_1を作成、IDとして3が割り振られた
        →続いてFD_2を作成すると、FD_2のIDは4となる
        →FD_1を破棄した上でFD_2を作成すると、FD_2のIDは3になる
        参考 → https://e-words.jp/w/ファイルディスクリプタ.html
    - select関数
        ソケット通信でよく利用される関数
        FDの状態を監視し、何かしらアクションがあった場合に対応する値を返す、という動作をする
        select関数の本質はI/Oの多重化なので、ソケット以外でも使える
        例)
        - ソケット通信
            ソケットにデータが届いたか監視する
        - 標準入力
            CUI上に何か入力が行われた監視する
        int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set exceptfds, struct timeval *timeout);
            - fd_set *readfds: 読み込み可能/不可能を監視したいFDの集合
                →集合の内、どれかが読み込み可能になるとselect関数は終了する
            - fd_set *writefds: 書き込み可能/不可能を監視したいFDの集合
                →集合の内、どれかが書き込み可能になるとselect関数は終了する
            - fd_set exceptfds: 例外の発生を監視したいFDの集合
                →集合の内、どれかに例外が発生するとselect関数は終了する
            - int nfds: 上記3つのFDの集合の内、FDの最大値+1
            - struct timeval *timeout: タイムアウト値(秒 + マイクロ秒) select関数が終了するまでの時間
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    /*
    標準入力に入力が発生した場合、その旨を知らせるプログラム
    */
    char inputval[16];
    int fd = 0;  // stdinのFD
    fd_set rfds;  // readfds
    struct timeval tv;  // timeout
    int retval;  // return value

    FD_ZERO(&rfds);  // FD集合を空にする
    FD_SET(fd, &rfds);  // FD集合(rfds)にstdin(ID = 0)を追加する
    // タイムアウト値の設定(5.5秒)
    tv.tv_sec = 5;  // 5秒
    tv.tv_usec = 500000;  // 0.5秒 = 500000マイクロ秒

    // rfdsに含まれているFDのうち、いずれかが読み込み可能になるかどうかを監視する
    // 5.5秒経ってもアクションがない場合、select関数は終了する
    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    if(retval < 0){
        // select関数はエラー時に-1を返す
        perror("select()");
    }
    else if(retval > 0){
        // select関数は成功時に3つのFD集合に含まれているFDの総数を返す
        // つまり、何かしらアクションを検知した場合は1以上の数値が返ってくると考える
        scanf("%s", inputval);
        printf("Input: %s\n", inputval);
    }
    else{
        // select関数はタイムアウトすると0を返す
        printf("timeout\n");
    }
    return (EXIT_SUCCESS);
}