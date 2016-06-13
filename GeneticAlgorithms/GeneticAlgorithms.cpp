#include "stdafx.h"
#include "GeneticAlgorithms.h"
int(*Pathptr)[2] = oliver30;	   //選擇路徑
//===================================================================================================
							   //定義路徑
typedef struct TagPathA {
	double FitnessValue;       //適應值Z
	double Length[SIZE];       //每個點的距離
	int Coor[SIZE];            //座標順序Coordinate
}TestPath;
IplImage *bestimage, *upload;
CvFont font1,font2;
//===================================================================================================
TestPath  Population[POPULATION];      //母體
TestPath  Pool[POPULATION];            //交配池
TestPath  BestPath;                    //最佳路徑
//===================================================================================================
template <typename AnyType>
void Swap(AnyType &n, AnyType &m)
{
	// 利用 call by reference 方式改變主函式的變數值
	AnyType temp;
	temp = n;
	n = m;
	m = temp;
}
// 計算適應值
void CalFitnessValue()
{
	int i, j;
	double SumLength[POPULATION] = { 0 };
	for (i = 0; i<POPULATION; i++) {
		for (j = 0; j<SIZE; j++)
			SumLength[i] += Population[i].Length[j];
		Population[i].FitnessValue = pow((1 / SumLength[i]), 20);  //長度分之一的n次方
	}
}
// 計算距離
void CalDistance()
{
	int i, j;
	for (i = 0; i<POPULATION; i++) {
		for (j = 0; j<SIZE - 1; j++)
			Population[i].Length[j] =
			sqrt((double)
				pow((double)
					Pathptr[Population[i].Coor[j + 1]][0] -		//x軸
					Pathptr[Population[i].Coor[j]][0], 2)
				+
				pow((double)
					Pathptr[Population[i].Coor[j + 1]][1] -		//y軸
					Pathptr[Population[i].Coor[j]][1], 2)
				);
		Population[i].Length[SIZE - 1] =
			sqrt((double)							//最後一個城鎮到第一個城鎮的距離
				pow((double)
					Pathptr[Population[i].Coor[SIZE - 1]][0] -
					Pathptr[Population[i].Coor[0]][0], 2)
				+
				pow((double)
					Pathptr[Population[i].Coor[SIZE - 1]][1] -
					Pathptr[Population[i].Coor[0]][1], 2)
				);
	}

}
// 隨機路徑
void RandomPath()
{
	int i, j, time, a;
	time = 0;
	int CoorList[SIZE] = { 0 };
	int Coor_1;
	for (i = 0; i<POPULATION; i++) {
		for (a = 0; a<SIZE; a++) {
			Coor_1 = (rand() % SIZE) + 1;
		check: //檢查重複
			for (j = 0; j<SIZE; j++) {
				if (CoorList[j] == Coor_1) {
					Coor_1 = (rand() % SIZE) + 1;
					goto check;
				}
			}
			//檢查end
			Population[i].Coor[time] = Coor_1 - 1; //座標存入struct
			CoorList[time] = Coor_1;
			time++;
		}
		time = 0;
		CalDistance();
		memset(CoorList, 0, sizeof(CoorList));    //初始化清單
		cout << "第" << i << "條路徑完成" << endl;
	}
	CalFitnessValue();
}
// 複製 競爭式 Tournament Selection (未完成)
void ReproductionTS()
{
	int i;
	double SumFitnessValue = 0.0;
	// 計算適應值的總和
	for (i = 0; i<POPULATION; i++)
		SumFitnessValue += Population[i].FitnessValue;
	for (i = 0; i<10; i++)
		cout << Population[i].FitnessValue / SumFitnessValue << endl;

}
// 複製 輪盤式 Roulette Wheel Selection
void ReproductionRWS()
{
	int i, length;
	double Rand;
	double SumFitnessValue = 0.0;
	double ProbAccu[POPULATION];  //機率累加Probability Accumulation

								  // 計算適應值的總和
	for (i = 0; i<POPULATION; i++)
		SumFitnessValue += Population[i].FitnessValue;

	// 羅盤建置累計機率
	ProbAccu[0] = Population[0].FitnessValue / SumFitnessValue;
	for (i = 1; i < POPULATION; i++)
		ProbAccu[i] =
		(Population[i].FitnessValue / SumFitnessValue) + (ProbAccu[i - 1]);


	//滾輪盤
	for (i = 0; i<POPULATION; i++) {  //轉POPULATION次
		Rand = (double)rand() / (double)RAND_MAX;
		for (length = 0; length<POPULATION; length++) {
			if (Rand <= ProbAccu[length]) {
				memcpy(&Pool[i], &Population[length], sizeof(TestPath));
				break;
			}
		}
	}

}
// 交配 Partially Matched Crossover
void CrossoverPMX()
{
	int rand1, rand2, pos;
	double cross_if;
	int i, j;
	int cnt = 0;
	int length = rand() % 10;
	int check = 0;
	int time = 0;

	int temp1[SIZE], temp2[SIZE];
	int ArrayTime1 = 0;
	int ArrayTime2 = 0;
	int *arr1 = (int *)malloc(length * sizeof(int));
	int *arr2 = (int *)malloc(10 * sizeof(int));
	//int arr2[8]={0};
	while (cnt < POPULATION) {
		rand1 = rand() % POPULATION;
		do { rand2 = rand() % POPULATION; } while (rand1 == rand2);
		cross_if = (double)rand() / (RAND_MAX + 1.0);
		if (cross_if>PC) {  //不交配 更正於 2012.06.15
			memcpy(&Population[cnt++], &Pool[rand1], sizeof(TestPath));
			memcpy(&Population[cnt++], &Pool[rand2], sizeof(TestPath));
		}
		else {

			memcpy(temp1, Pool[rand1].Coor, SIZE*sizeof(int)); //暫存
			memcpy(temp2, Pool[rand2].Coor, SIZE*sizeof(int));
			pos = 0;//rand()%(29-length);

					//step1 片段交換
			for (j = 0; j<length; j++) {
				for (i = 0; i<length; i++) {
					if (temp1[j + pos] != temp2[i + pos]) check++;
					if (check == length) arr1[time++] = temp1[j + pos];
				}
				check = 0;
			}
			time = 0;//初始
			for (j = 0; j<length; j++) {
				for (i = 0; i<length; i++) {
					if (temp2[j + pos] != temp1[i + pos]) check++;
					if (check == length) arr2[time++] = temp2[j + pos];
				}
				check = 0;
			}
			time = 0;

			for (i = 0; i<length; i++) {
				Swap(temp1[i + pos], temp2[i + pos]);
			}


			//step2  刪除並填入
			for (i = length; i<SIZE; i++) {
				for (j = 0; j<length; j++) {
					if (temp1[i] == temp1[j]) {
						temp1[j] = arr1[ArrayTime1++];
					}
					if (temp2[i] == temp2[j]) {
						temp2[j] = arr2[ArrayTime2++];
					}
				}
			}
			ArrayTime1 = ArrayTime2 = 0;
			//step3 複製進population
			memcpy(Population[cnt++].Coor, temp1, SIZE*sizeof(int));
			memcpy(Population[cnt++].Coor, temp2, SIZE*sizeof(int));
			CalDistance();
			CalFitnessValue();
		}
	}
	free(arr2);
	free(arr1);
}

//交配 混和式GA(Hybrid Genetic Algorithm)(考慮)
void CrossoverHGA()
{
	int i, cnt = 0;
	int pos = NULL;
	int rand1, rand2;
	int City1, City2;
	double crossover_if;
	int ignore[SIZE];	//忽略陣列
	int	skipnum1 = 0, skipnum2 = 0;		//跳過的次數
	int num = 0;
	int rand1length, rand2length;
	while (cnt < POPULATION) {
		rand1 = rand() % POPULATION;// 隨機選二個個體
		do { rand2 = rand() % POPULATION; } while (rand2 == rand1);
		crossover_if = (double)rand() / (RAND_MAX + 1.0);
		if (crossover_if > PC) {
			// 不交配, 將交配池中之個體丟回母體
			memcpy(&Population[cnt++], &Pool[rand1], sizeof(TestPath));
			memcpy(&Population[cnt++], &Pool[rand2], sizeof(TestPath));
		}
		else {
			do { pos = rand() % SIZE; } while (pos == NULL);
			Population[cnt].Coor[0] = pos;						//固定第一個城市的位置
			for (i = 1; i < SIZE; i++) {						//找出第二個程式

				for (int j = 0; j < SIZE; j++) {				//找出指定城市在基因的位置
					if (pos == Pool[rand1].Coor[j])City1 = j;
					if (pos == Pool[rand2].Coor[j])City2 = j;
				}
				for (int k = 0; k<SIZE; k++)
					for (int l = 0; l<num; l++)
						if (Pool[rand1].Coor[City1 + 1 + skipnum1] == ignore[l])skipnum1++;
				for (int k = 0; k<SIZE; k++)
					for (int l = 0; l<num; l++)
						if (Pool[rand2].Coor[City2 + 1 + skipnum2] == ignore[l])skipnum2++;
				if (skipnum1 | skipnum2) {
					rand1length = sqrt((double)pow((double)Pathptr[Pool[rand1].Coor[City1 + 1 + skipnum1]][0] - Pathptr[Pool[rand1].Coor[City1]][0], 2)
						+ pow((double)Pathptr[Pool[rand1].Coor[City1 + 1 + skipnum1]][1] - Pathptr[Pool[rand1].Coor[City1]][1], 2));
					rand2length = sqrt((double)pow((double)Pathptr[Pool[rand2].Coor[City2 + 1 + skipnum2]][0] - Pathptr[Pool[rand2].Coor[City2]][0], 2)
						+ pow((double)Pathptr[Pool[rand2].Coor[City2 + 1 + skipnum2]][1] - Pathptr[Pool[rand2].Coor[City2]][1], 2));
					Population[cnt].Coor[i] =
						(rand1length>rand2length)
						? Pool[rand2].Coor[City2 + 1 + skipnum2]
						: Pool[rand1].Coor[City1 + 1 + skipnum1]
						;
				}
				else {
					(Population[cnt].Coor[i] =
						Pool[rand1].Length[City1]>Pool[rand2].Length[City2])
						? Pool[rand2].Coor[City2 + 1]
						: Pool[rand1].Coor[City1 + 1];
				}
				pos = Population[cnt].Coor[i];					//更新下個城市
				ignore[num++] = pos;
				skipnum1 = skipnum2 = 0;
				//bug 1刪除會使交配池變形 考慮用忽略的方法 
			}
			num = 0;
			memset(ignore, '\0', sizeof(int));

			/*
			for (i = 0; i<pos; i++) {
			Population[cnt].Coor[i] = Pool[rand1].Coor[i];
			Population[cnt + 1].Coor[i] = Pool[rand2].Coor[i];
			}
			for (i = pos; i<SIZE; i++) {
			Population[cnt + 1].Coor[i] = Pool[rand1].Coor[i];
			Population[cnt].Coor[i] = Pool[rand2].Coor[i];
			}*/
			cnt += 1; // 已複制完一條
		}
	}
}
void Mutation() //Inversion Mutation
{
	int i;
	int pos1, pos2;
	for (i = 0; i<POPULATION; i++) {
		double mutation_if;								//突變機率
		mutation_if = (double)rand() / (RAND_MAX + 1.0);		//[0,1)  包含0不包含1
																//cout << mutation_if << "\n";
		if (mutation_if <= PM) {
			//cout << "第" << i << "個母體進入突變\n";
			pos1 = rand() % (SIZE - 1);  // 突變位置
			do { pos2 = rand() % (SIZE - 1); } while (pos1 == pos2);
			if (pos2>pos1)   pos2 ^= pos1 ^= pos2 ^= pos1;		//pos1>pos2 需要交換 做四次xor
			while ((pos1 - pos2) >= 1) {
				//Population[i].Coor[pos1]^= Population[i].Coor[pos2]^=Population[i].Coor[pos1]^=Population[i].Coor[pos2]; 
				Swap(Population[i].Coor[pos1], Population[i].Coor[pos2]);
				pos1 -= 1;
				pos2 += 1;
			}
		}
		// 突變完後再算一次母體適應值
		CalDistance();
		CalFitnessValue();
	}

}
void UpdateBestPath(int GenerationTime) {
	for (int i = 0; i<POPULATION; i++) {
		double SumLength = 0;
		if (Population[i].FitnessValue > BestPath.FitnessValue) {
			memcpy(&BestPath, &Population[i], sizeof(TestPath));
			for (int j = 0; j<SIZE; j++) {
				SumLength += BestPath.Length[j];
			}
			cout << "第" << GenerationTime << "代";
			cout << fixed << setprecision(10) << SumLength << endl;
			//for(int k=0;k<30;k++) cout <<"["<< BestPath.Coor[k]+1<<"]->";  //最佳路徑顯示
			//cout<<endl;
		}
	}
}
void InitOpencvConfig()
{
	bestimage = cvCreateImage(cvSize(600, 600), IPL_DEPTH_8U, 3);
	cvSet(bestimage, cvScalar(255, 255, 255));
	//cvStartWindowThread();
	//cvNamedWindow("Best", CV_WINDOW_AUTOSIZE);
	//cvShowImage("Best Path", bestimage);
	cvInitFont(&font1, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0,1.5,4);
	cvInitFont(&font2, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, 8);
}
void drawBest(int generation)
{
	double SumLength = 0;
	stringstream strs1, strs2,time;
	for (int i = 0; i<SIZE - 1; i++) {
		time << i;
		cvLine(bestimage, cvPoint(5 * Pathptr[BestPath.Coor[i]][0], 5 * Pathptr[BestPath.Coor[i]][1]),
			cvPoint(5 * Pathptr[BestPath.Coor[i + 1]][0], 5 * Pathptr[BestPath.Coor[i + 1]][1]), CV_RGB(0, 0, 0), 1, CV_AA, 0);
		cvCircle(bestimage, cvPoint(5 * Pathptr[BestPath.Coor[i]][0], 5 * Pathptr[BestPath.Coor[i]][1]), 1, CV_RGB(0, 0, 255), 5, 8, 0);
		cvPutText(bestimage, time.str().c_str(),cvPoint(5 * Pathptr[BestPath.Coor[i]][0]+5, 5 * Pathptr[BestPath.Coor[i]][1]+5), &font1, CV_RGB(0, 0, 0));
		time.str("");
	}
	time << SIZE-1;
	cvPutText(bestimage, time.str().c_str(), cvPoint(5 * Pathptr[BestPath.Coor[SIZE - 1]][0]+5, 5 * Pathptr[BestPath.Coor[SIZE - 1]][1]+5), &font1, CV_RGB(0, 0, 0));
	cvCircle(bestimage, cvPoint(5 * Pathptr[BestPath.Coor[SIZE-1]][0], 5 * Pathptr[BestPath.Coor[SIZE-1]][1]), 1, CV_RGB(0, 0, 255), 5, 8, 0);
	cvLine(bestimage, cvPoint(5 * Pathptr[BestPath.Coor[0]][0], 5 * Pathptr[BestPath.Coor[0]][1]),
		cvPoint(5 * Pathptr[BestPath.Coor[SIZE - 1]][0], 5 * Pathptr[BestPath.Coor[SIZE - 1]][1]), CV_RGB(0, 0, 0), 1, CV_AA, 0);

	for (int j = 0; j<SIZE; j++) {
		SumLength += BestPath.Length[j];
	}
	strs1 << "Generation: " << generation + 1;
	strs2 << fixed << setprecision(6) << "Distance: " << SumLength;

	cvPutText(bestimage, strs1.str().c_str(), cvPoint(20, 20), &font2, CV_RGB(255, 0, 0));
	cvPutText(bestimage, strs2.str().c_str(), cvPoint(20, bestimage->height - 20), &font2, CV_RGB(255, 0, 0));

}
//===================================================================================================
//  主程式
int main(int argc, char** argv) {


	srand((unsigned int)time(NULL));
	cout << "隨機路徑生成..." << endl;
	RandomPath();
	cout << "隨機路徑完成" << endl;


	cout << "開始基因演替" << endl;
	for (int i = 0; i<GENERATION; i++) {
		InitOpencvConfig();
		//cout << i << "代\n";
		ReproductionRWS();
		CrossoverPMX();
		Mutation();
		UpdateBestPath(i);

		//Drawd  Map--------------------------------------

		drawBest(i);
		upload = cvCloneImage(bestimage);
		cvShowImage("Path", upload);
		cvReleaseImage(&bestimage);
		cvReleaseImage(&upload);
		cvWaitKey(100);
	}


	cvWaitKey(0);
	return 0;
}
//===================================================================================================