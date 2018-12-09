#include "ImgSeg.h"

double getWeightedGS(vector<int> hist, int from, int to) {
	int gsSum = 0, gsSumW = 0;
	int idxHist = from;

	for (idxHist = from; idxHist <= to; ++idxHist) {
		gsSum += hist[idxHist];
		gsSumW += hist[idxHist] * idxHist;
	}
	if ((!gsSum) || (!gsSumW)) return 0.0;
	else return static_cast<double>(gsSumW) / static_cast<double>(gsSum);
}

vector<int> getHist(Mat in) {
	int idxCol = 0, idxRow = 0;
	int imgCol = in.cols, imgRow = in.rows;
	vector<int> hist(256, 0);

	for (idxRow = 0; idxRow < imgRow; ++idxRow) {
		for (idxCol = 0; idxCol < imgCol; ++idxCol) {
			++hist[in.at<unsigned char>(idxRow, idxCol)];
		}
	}
	return hist;
}

vector<double> getHistNorm(vector<int> hist) {
	int idxHist = 0;
	int sum = 0;
	vector<double> histNorm(256, 0.0);

	for (idxHist = 0; idxHist < 256; ++idxHist) {
		sum += hist[idxHist];
	}
	for (idxHist = 0; idxHist < 256; ++idxHist) {
		histNorm[idxHist] = static_cast<double>(hist[idxHist]) / static_cast<double>(sum);
	}
	return histNorm;
}

Mat segMan(Mat in, int thres) {
	int idxCol = 0, idxRow = 0;
	int imgCol = in.cols, imgRow = in.rows;
	Mat out = in.clone();

	for (idxRow = 0; idxRow < imgRow; ++idxRow) {
		for (idxCol = 0; idxCol < imgCol; ++idxCol) {
			if (out.at<unsigned char>(idxRow, idxCol) <= thres)
				out.at<unsigned char>(idxRow, idxCol) = 0;
			else if (out.at<unsigned char>(idxRow, idxCol) > thres)
				out.at<unsigned char>(idxRow, idxCol) = 255;
		}
	}
	return out;
}

sbgRet segBasicG(Mat in, double accu) {
	double thresD = 0.0, thresGP = 0.0, thresGC = 0.0;
	double gsAvg1 = 0.0, gsAvg2 = 0.0;
	int idxHist = 0;
	int histMin = in.size().height*in.size().width, histMinIdx = 0;
	vector<int> hist = getHist(in);
	vector<int> histValleyLoc;
	Mat out = Mat::zeros(in.rows, in.cols, CV_8UC1);
	sbgRet ret;

	for (idxHist = 0; idxHist < 256; ++idxHist) {
		if ((hist[idxHist]) && (hist[idxHist] < histMin)) {
			histMin = hist[idxHist];
			histMinIdx = idxHist;
		}
	}
	for (idxHist = 0; idxHist < 256; ++idxHist) {
		if ((hist[idxHist] >= histMin) && (hist[idxHist] <= histMin + INT_BASICGB_MINPLUS))
			histValleyLoc.push_back(idxHist);
	}
	thresGC = histValleyLoc[histValleyLoc.size() / 2];
	thresD = fabs(thresGC - thresGP);
	while (fabs(thresD) > 1E-6) {
		thresGP = thresGC;
		gsAvg1 = getWeightedGS(hist, static_cast<int>(thresGP) + 1, 255);
		gsAvg2 = getWeightedGS(hist, 0, static_cast<int>(thresGP));
		thresGC = (gsAvg1 + gsAvg2) / 2.0;
		thresD = fabs(thresGC - thresGP);
	}
	out = segMan(in, static_cast<int>(thresGC));
	ret.out = out;
	ret.threshold = static_cast<int>(thresGC);
	return ret;
}

soRet segOtsu(Mat in) {
	double mG_GIM = 0.0, kAsterisk = 0.0, etaAsterisk = 0.0;
	double SigmaBk_MaxV = 0.0, SigmaG_GV = 0.0;
	int SigmaBk_MaxI = 0, kSum = 0;
	vector<double> histNorm = getHistNorm(getHist(in));
	vector<double> P1k_CS(256, 0.0), mk_CM(256, 0.0), SigmaBk_BCV(256, 0.0), SigmaBk_Max;
	Mat out = Mat::zeros(in.rows, in.cols, CV_8UC1);
	soRet ret;

	for (int k = 0; k < 256; ++k) {
		double sumPi = 0.0;
		double sumiPi = 0.0;
		for (int i = 0; i < k; ++i) {
			sumPi += histNorm[i];
			sumiPi += i * histNorm[i];
		}
		P1k_CS[k] = sumPi;
		mk_CM[k] = sumiPi;
		mG_GIM += k * histNorm[k];
	}
	for (int k = 0; k < 256; ++k) {
		SigmaBk_BCV[k] = pow(mG_GIM*P1k_CS[k] - mk_CM[k], 2) / (P1k_CS[k] * (1 - P1k_CS[k]));
		SigmaG_GV += pow(k - mG_GIM, 2)*histNorm[k];
	}
	for (int k = 0; k < 256; k++) {
		if (SigmaBk_BCV[k] > SigmaBk_MaxV) {
			SigmaBk_MaxV = SigmaBk_BCV[k];
			SigmaBk_MaxI = k;
		}
	}
	for (int k = 0; k < 256; k++) {
		if (SigmaBk_BCV[k] == SigmaBk_MaxV) {
			SigmaBk_Max.push_back(k);
			kSum += k;
		}
	}
	kAsterisk = static_cast<double>(kSum / SigmaBk_Max.size());
	etaAsterisk = SigmaBk_BCV[static_cast<int>(kAsterisk)] / SigmaG_GV;
	out = segMan(in, static_cast<int>(kAsterisk));
	ret.out = out;
	ret.threshold = static_cast<int>(kAsterisk);
	ret.separability = etaAsterisk;
	return ret;
}

ssRet segPart(Mat in) {
	int rowL = 0, rowR = 0, colD = 0, colU = 0;
	vector<int> subCol = { in.cols / 3,in.cols / 3,in.cols - in.cols * 2 / 3 };
	vector<int> subRow = { in.rows / 2,in.rows - in.rows / 2 };
	Mat sub;
	soRet retSO;
	ssRet ret;

	ret.out = Mat::zeros(in.rows, in.cols, CV_8UC1);
	for (int r = 0; r < subRow.size(); ++r) {
		rowR += subRow[r];
		rowL = rowR - subRow[r];
		colU = 0;
		colD = 0;
		for (int c = 0; c < subCol.size(); ++c) {
			colU += subCol[c];
			colD = colU - subCol[c];
			sub = Mat::zeros(subRow[r], subCol[c], CV_8UC1);
			for (int i = rowL, m = 0; i < rowR; ++i, ++m) {
				for (int j = colD, n = 0; j < colU; ++j, ++n) {
					sub.at<unsigned char>(m, n) = in.at<unsigned char>(i, j);
				}
			}
			retSO = segOtsu(sub);
			sub = retSO.out;
			ret.thres.push_back(retSO.threshold);
			ret.sep.push_back(retSO.separability);
			for (int i = rowL, m = 0; i < rowR; ++i, ++m) {
				for (int j = colD, n = 0; j < colU; ++j, ++n) {
					ret.out.at<unsigned char>(i, j) = sub.at<unsigned char>(m, n);
				}
			}
		}
	}
	return ret;
}