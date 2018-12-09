#include "ImgSeg.h"

int main(void) {
	int inputMode = 0, imgSegThres = 0;
	char imgLoc[1024];
	Mat imgIn, imgOut;
	sbgRet retM2;
	soRet retM3;
	ssRet retM4;

	while (inputMode != 5) {
		system("cls");
		cout << "Image Segmentation - Thresholding\n" << endl;
		cout << "Locate image file (drag supported)[Q - quit]: ";
		cin.clear();
		cin.getline(imgLoc, 1024);
		if (!strcmp(imgLoc, "Q") || !strcmp(imgLoc, "q")) break;
		imgIn = imread(imgLoc, IMREAD_GRAYSCALE);
		imgOut = Mat::zeros(imgIn.rows, imgIn.cols, CV_8UC1);
		if (imgIn.empty()) {
			cerr << "[Error] No image is loaded." << endl;
			system("pause > nul");
			continue;
		}
		cout << "\nSelect a thresholding method:" << endl;
		cout << "[1] Providing a threshold manually" << endl;
		cout << "[2] Basic global thresholding" << endl;
		cout << "[3] Optimum global thresholding using Otsu¡¯s method" << endl;
		cout << "[4] Variable thresholding by image partitioning (2 * 3)" << endl;
		cout << "Select a method [1-4][5 - quit]: ";
		cin >> inputMode;
		putchar('\n');
		switch (inputMode) {
		case 1:
			cout << "Provide a threshold [0-255]: ";
			cin >> imgSegThres;
			if (!((imgSegThres >= 0) && (imgSegThres <= 255))) {
				cerr << "[Error] Threshold is out of range." << endl;
				GOTO;
			}
			imgOut = segMan(imgIn, imgSegThres);
			break;
		case 2:
			retM2 = segBasicG(imgIn, DBL_BASICGB_ACCU);
			imgOut = retM2.out;
			cout << "[Info] Segmentation is done with threshold " << retM2.threshold << endl;
			break;
		case 3:
			retM3 = segOtsu(imgIn);
			imgOut = retM3.out;
			printf("[Info] Segmentation is done with threshold %d and separability %.3f.\n",
				retM3.threshold, retM3.separability);
			break;
		case 4:
			retM4 = segPart(imgIn);
			imgOut = retM4.out;
			cout << "[Info] Segmentation is done with following thresholds: ";
			for (int i = 0; i < 6; ++i) {
				printf("%2d  ", retM4.thres[i]);
			}
			cout << "\n       and following separabilities respectively: ";
			for (int i = 0; i < 6; ++i) {
				printf("%.3f  ", retM4.sep[i]);
			}
			putchar('\n');
			break;
		case 5:
			break;
		default:
			cerr << "[Error] No such mode." << endl;
			GOTO;
		}
		namedWindow(STR_WINNAME, WINDOW_AUTOSIZE);
		imshow(STR_WINNAME, imgOut);
		waitKey(0);
		destroyWindow(STR_WINNAME);
	}
	return 0;
}