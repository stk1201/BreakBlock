#include<stdio.h>
#include<stdlib.h>
#include<eggx.h>
#include<time.h>
#include<string.h>
#include<unistd.h>

//当たり判定
int atariU(double x1, double y1,double r1,double x2, double y2,double w2,double h2);
int atariD(double x1, double y1,double r1,double x2, double y2,double w2,double h2);
int atariL(double x1, double y1,double r1,double x2, double y2,double w2,double h2);
int atariR(double x1, double y1,double r1,double x2, double y2,double w2,double h2);

//音声ファイルの定義付け
#define SND_REF "maoudamashi8bit.mp3"
int PlaySound(char *file){//音楽開始定義
	pid_t pid;//PID型宣言
	pid=fork();//子プロセス開始
	if(pid<0){//子プロセス開始不可
		perror("forkerror");
		exit(-1);
	}//子プロセス開始できたら、その上で再生コマンド
	else if(pid==0){
		execlp("play","play",file,NULL);
		perror("playerror");
		exit(-1);
	}
	return pid;
}
void StopSound(int PID){
	static cmd[256];
	sprintf(cmd, "kill %d",PID);
	system(cmd);
	
	return;
}


int main() {
	int win,block_x,block_y,u,key,count,ready,bar_cont,click_btn,click_type,major,random,pid;
	double bar_x, bar_y, bar_width, ball_x, ball_y, ball_vx, ball_vy, timer,click_x,click_y;
	int block[10][10]={{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1}};
	win=gopen(640, 480);//画面表示
	winname(win,"Break the Block!!");
	layer(win,0,1);//レイヤーの初期値
	
	newrgbcolor(win, 255, 255, 255);//色
	
	pid=PlaySound(SND_REF);
	
	//ready画面
	while(1){
		drawstr(win,270,240,30,0,"READY??");
		drawstr(win,250,200,30,0,"(Press Enter)");
		drawstr(win,450,450,30,0,"<CONTROL>\n-> or d: right\n<- or a: left");
		copylayer(win,1,0);
		key=ggetch();
		if(key==0x00d){
			gclr(win);
			ready=0;
			break;
		}
	}
	
	major=1;
	
	for(timer=0;timer<10000000;timer=timer+0.1){}//時間停止
	
	while(1){
		if(major==1){
			bar_x=320;
			bar_y=50;
			
			bar_width=80;
			
			ball_x=320;
			ball_y=100;
			
			ready=0;
			ball_vx=0.1;//ボールのx方向動く量
			ball_vy=-0.25;//ボールのy方向動く量
			
			gsetnonblock(ENABLE);//入力と関係なくボール移動させる
			u=0;
			
			srand((unsigned int)time(NULL));
			
			//ブロック表示決め
			for(block_y=0;block_y<10;block_y++){
				for(block_x=0;block_x<10;block_x++){
					random=rand()%2;
					block[block_x][block_y]=random;
				}
			}
			
			
			
			while(1){
				//after ready
				if(ready==0&&atariU(ball_x,ball_y,3,bar_x,bar_y,bar_width,10)==1){
					ball_vx=0.5;//ボールのx方向動く量
					ball_vy=-1.25;//ボールのy方向動く量
					ready=1;
				}
				
				
				gclr(win);//画面消しコマ撮り防止
				
				bar_cont=ggetch();
				if(bar_cont==0x01c||bar_cont==0x064){
					bar_x=bar_x+10.0;//バーとキーボートの同期
				}
				if(bar_cont==0x01d||bar_cont==0x061){
					bar_x=bar_x-10.0;
				}
				
				fillrect(win,bar_x-bar_width/2,bar_y-5, bar_width, 10);//バー表示
				
				fillcirc(win,ball_x,ball_y,3,3);//ボール表示
				
				//ブロック表示
				for(block_y=0;block_y<10;block_y++){
					for(block_x=0;block_x<10;block_x++){
						if(block[block_y][block_x]==1){
							if(block_y==0){
								newrgbcolor(win,255,0,0);
							}
							if(block_y==1){
								newrgbcolor(win,255,140,0);
							}
							if(block_y==2){
								newrgbcolor(win,255,215,0);
							}
							if(block_y==3){
								newrgbcolor(win,173,255,47);
							}
							if(block_y==4){
								newrgbcolor(win,0,255,0);
							}
							if(block_y==5){
								newrgbcolor(win,0,206,209);
							}
							if(block_y==6){
								newrgbcolor(win,0,255,255);
							}
							if(block_y==7){
								newrgbcolor(win,0,0,255);
							}
							if(block_y==8){
								newrgbcolor(win,0,0,205);
							}
							if(block_y==9){
								newrgbcolor(win,128,0,128);
							}
							
							fillrect(win,20+(60*block_x)+1,460-(20*block_y)+1,58,18);
							newrgbcolor(win, 255, 255, 255);
						}
						if(block[block_y][block_x]==0){
							newrgbcolor(win, 0, 0, 0);
							fillrect(win,20+(60*block_x)+1,460-(20*block_y)+1,58,18);
							newrgbcolor(win, 255, 255, 255);
						}
						//ブロック消し
						if(block[block_y][block_x]==1&&atariU(ball_x,ball_y,3, 21+60*block_x+29,460-(20*block_y)-9,58,18)==1){
							ball_vy=ball_vy*(-1.01);
							ball_vx=ball_vx*(1.01);
							block[block_y][block_x]=0;
						}
						if(block[block_y][block_x]==1&&atariD(ball_x,ball_y,3, 21+60*block_x+29,460-(20*block_y)-9,58,18)==1){
							ball_vy=ball_vy*(-1.01);
							ball_vx=ball_vx*(1.01);
							block[block_y][block_x]=0;
							
						}
						if(block[block_y][block_x]==1&&atariR(ball_x,ball_y,3, 21+60*block_x+29,460-(20*block_y)-9,58,18)==1){
							ball_vx=ball_vx*(-1.01);
							ball_vy=ball_vy*(1.01);
							block[block_y][block_x]=0;
						}
						if(block[block_y][block_x]==1&&atariL(ball_x,ball_y,3, 21+60*block_x+29,460-(20*block_y)-9,58,18)==1){
							ball_vx=ball_vx*(-1.01);
							ball_vy=ball_vy*(1.01);
							block[block_y][block_x]=0;
						}
					}
				}		
				
				//clear or game over
				count=0;
				for(block_y=0;block_y<10;block_y++){
					for(block_x=0;block_x<10;block_x++){
						if(block[block_y][block_x]==1){
							count=count+1;
						}
					}
				}
				if(count==0){
					u=2;
					major=2;
					break;
				}
				
				
				//壁当て
				if(ball_y+4 > 479){
					ball_vy=ball_vy*(-1.0);
				}
				
				if(ball_y-4 < 0){
					u=1;
					major=2;
					break;
				}
				
				if(ball_x+4 > 639){
					ball_vx=ball_vx*(-1.0);
				}
				
				if(ball_x-4 < 0){
					ball_vx=ball_vx*(-1.0);
				}
				
				//バー当たり判定
				if(atariU(ball_x,ball_y,3,bar_x,bar_y,bar_width,10)==1){
					ball_vy=ball_vy*(-1.0);
					ball_y=bar_y+10;
				}
				
				
				
				
				ball_x=ball_x+ball_vx;
				ball_y=ball_y+ball_vy;
				
				for(timer=0 ; timer<100000 ; timer=timer+0.1){}//ボールの表示速度の調整
				
				copylayer(win,1,0);//見えるレイヤーにコピー
				
			}
			
			for(timer=0;timer<10000000;timer=timer+0.1){}//時間停止
			
			if(u==1){
				StopSound(pid);
				while(1){
					gclr(win);
					drawstr(win,270,250,30,0,"GAME OVER");
					fillrect(win,270,170,100,50);
					newrgbcolor(win, 0, 0, 0);
					drawstr(win,275,190,20,0,"Try again");
					newrgbcolor(win, 255, 255, 255);
					copylayer(win,1,0);
					if(win==ggetevent(&click_type,&click_btn,&click_x,&click_y)){
						if(click_type==ButtonPress&&270<=click_x&&click_x<=370&&170<=click_y&&click_y<=220){
							u=0;
							major=1;
							pid=PlaySound(SND_REF);
							break;
						}
					}
				}
			}
			if(u==2){
				StopSound(pid);
				while(2){
					gclr(win);
					drawstr(win,270,230,30,0,"CLEAR!!");
					copylayer(win,1,0);	
				}
			}
		}
	}
	
	
	
	
	
	exit(0);
}
int atariU(double x1, double y1,double r1,double x2, double y2,double w2,double h2){
	if(y1-(r1+1) < y2+h2/2 && y1-(r1+1) > y2-h2/2){
		if(x1 > x2-w2/2 && x1 < x2+w2/2){
			return 1;
		}
	}
	return 0;
}

int atariD(double x1, double y1,double r1,double x2, double y2,double w2,double h2){
	if(y1+(r1+1)>y2-h2/2 && y1+(r1+1)<y2+h2/2){
		if(x1 > x2-w2/2 && x1 < x2+w2/2){
			return 1;
		}
	}
	return 0;
}

int atariL(double x1, double y1,double r1,double x2, double y2,double w2,double h2){
	if(y1<y2+h2/2 && y1>y2-h2/2){
		if(x1+(r1+1)>x2-w2/2&&x1+(r1+1)<x2+w2){
			return 1;
		}
	}
	return 0;
}

int atariR(double x1, double y1,double r1,double x2, double y2,double w2,double h2){
	if(y1<y2+h2/2 && y1>y2-h2/2){
		if(x1-(r1+1)<x2+w2/2 && x1+(r1+1)>x2-w2/2){
			return 1;
		}
	}
	return 0;
}
