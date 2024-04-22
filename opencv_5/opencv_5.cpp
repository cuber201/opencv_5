// opencv_5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;

string find_features(const Mat& img_crop, vector<string>& cardname, vector<Mat>& card) {
    Ptr<SIFT> sift = SIFT::create();
    double min[2] = {10000,0};
    vector<KeyPoint> keypoints1;
    Mat descriptors1;
    sift->detectAndCompute(img_crop, noArray(), keypoints1, descriptors1);

    for (int i = 0; i < card.size(); i++) {
        vector<KeyPoint>  keypoints2;
        Mat descriptors2;
        
        sift->detectAndCompute(card[i], noArray(), keypoints2, descriptors2);

        BFMatcher matcher(NORM_L2);
        vector<DMatch> matches;
        matcher.match(descriptors1, descriptors2, matches);

        double similarity = 0;

        for (int i = 0; i < matches.size(); ++i) {
            similarity += matches[i].distance;
        }

        similarity /= matches.size();
        if (min[0] > similarity){
            min[0] = similarity;
            min[1] = i;
        }
        cout << "Similarity between image1 and image2: " << similarity<<"   "<<min[0]<<" "<<min[1] << endl;
        
    }
    return cardname[min[1]];
}

int main()
{
    string imagePath = "cards.jpg";
    Mat cards = imread(imagePath);
    Mat clone = cards.clone();
    if (cards.empty())
    {
        cout << "Image not found." << endl;
        return -1;
    }
    vector<string> cardname = { "6_chervi","10_kresti","valet_piki","8_chervi","korol_chervi" };
    vector<Mat> card;
   
    for (int i = 0; i < cardname.size(); i++) {
        Mat temp = imread(cardname[i] + ".jpg");
        if (temp.empty())
        {
            cout << "Image not found." << endl;
            return -1;
        }
        card.push_back(temp);
    }



    Mat  cards_hsv, mask, thresh;
    cvtColor(cards, cards_hsv, COLOR_BGR2HSV);
    inRange(cards_hsv, Scalar(0, 0, 160), Scalar(255, 55, 255), mask);
    //GaussianBlur(mask, mask, Size(5, 5), 0);
    threshold(mask, thresh, 100, 255, THRESH_BINARY);
    vector<vector<Point>> contours;
    findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    drawContours(cards, contours, -1, Scalar(0, 255, 0), 2);

    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        int x = rect.x;
        int y = rect.y;
        int w = rect.width;
        int h = rect.height;
        if (w > 20 && h > 30) {
            Mat img_crop = clone(Range(y - 15, y + h + 15), Range(x - 15, x + w + 15));
            if (img_crop.size().height < img_crop.size().width)
                rotate(img_crop, img_crop, ROTATE_90_CLOCKWISE);
            imshow("cards " + i, img_crop);
            string cards_name = find_features(img_crop, cardname, card); 
            putText(cards, cards_name, Point(x, y + 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 2);
        }
    }


    //imshow("show window_2", mask);
    //imshow("show windo", thresh);
    imshow("show window", cards);
    waitKey(0);
    return 0;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
