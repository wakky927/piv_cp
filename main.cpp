#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace boost::property_tree;
using namespace cv;
using namespace std;

static void meshgrid(const int &wid, const int &hei, const int &nx, const int &ny,
                     Mat1i &X, Mat1i &Y);
static void piv(const Mat1f &ma, const Mat1f &mb, Mat1f &dX, Mat1f &dY, const int &wid, const int &hei, const int &t,
                const int &s, const int &gx, const int &gy, const int &th);

int main(int argc, char* argv[]) {
    // program start!
    cout << "\nprogram start!" << endl;

    // read config file
    ptree pt;
    if (argc < 1) {
        read_ini(argv[1], pt);
    } else {
        read_ini("../test.ini", pt);
    }


    string dir_in = pt.get_optional<string>("Path.DIR_IN").get();
    string dir_out = pt.get_optional<string>("Path.DIR_OUT").get();

    int i_start = pt.get_optional<int>("i.start").get();
    int i_end = pt.get_optional<int>("i.end").get();
    int i_iter = pt.get_optional<int>("i.iter").get();

    int Gx = pt.get_optional<int>("Grid.x").get();
    int Gy = pt.get_optional<int>("Grid.y").get();

    int t_size = pt.get_optional<int>("Window.t").get();
    int s_size = pt.get_optional<int>("Window.s").get();

    int th = pt.get_optional<int>("Params.threshold").get();

    stringstream ia_path, ib_path, dx_path, dy_path;

    chrono::system_clock::time_point start, end;

    for (int i = i_start; i < i_end + 1; i += i_iter) {
        start = chrono::system_clock::now();

        // set img path
        ia_path << dir_in << setw(8) << setfill('0') << i << ".bmp";
        ib_path << dir_in << setw(8) << setfill('0') << i + i_iter
                   << ".bmp";

        // read img
        Mat1f ia = imread(ia_path.str(), 0);
        Mat1f ib = imread(ib_path.str(), 0);

        // check mat
        if (!ia.data || !ib.data) {
            cout << "No image data" << endl;
            return 1;
        }

        // padding
        int wid = ia.cols;
        int hei = ia.rows;
        int padding = s_size + t_size;
        Mat ia_p, ib_p;

        ia_p.create(ia.rows + 2 * padding, ia.cols + 2 * padding, ia.type());
        ib_p.create(ib.rows + 2 * padding, ib.cols + 2 * padding, ib.type());
        ia_p.setTo(Scalar::all(0));
        ib_p.setTo(Scalar::all(0));

        ia.copyTo(ia_p(Rect(padding, padding, ia.cols, ia.rows)));
        ib.copyTo(ib_p(Rect(padding, padding, ib.cols, ib.rows)));

        // set result
        float nan = numeric_limits<float>::signaling_NaN();
        Mat1f dX = Mat::ones(Gy, Gx, CV_32F) * nan;
        Mat1f dY = Mat::ones(Gy, Gx, CV_32F) * nan;

        // cross correlation
        piv(ia_p, ib_p, dX, dY, wid, hei, t_size, s_size, Gx, Gy, th);

        // save result
        dx_path << dir_out << "dx_" << i << "_" << i + i_iter << ".csv";
        dy_path << dir_out << "dy_" << i << "_" << i + i_iter << ".csv";

        ofstream ofs_dx(dx_path.str());
        ofstream ofs_dy(dy_path.str());

        ofs_dx << format(dX, Formatter::FMT_CSV) << endl;
        ofs_dy << format(dY, Formatter::FMT_CSV) << endl;

        // free ss
        ia_path.str("");
        ib_path.str("");
        dx_path.str("");
        dy_path.str("");

        // calc process time
        end = chrono::system_clock::now();
        double time = static_cast<double>(chrono::duration_cast<chrono::seconds>(end - start).count());
        cout << "no." << i << " & no." << i + i_iter << "    fin.  time: " << fixed << setprecision(3) << time
             << "[s]" << endl;
    }

    cout << "\nprogram end." << endl;

    return 0;
}


static void meshgrid(const int &wid, const int &hei, const int &nx, const int &ny,
                     Mat1i &X, Mat1i &Y) {
    vector<int> x, y;
    for (int i = Range(0,wid).start; i <= Range(0,wid).end; i += nx) x.push_back(i);
    for (int i = Range(0,hei).start; i <= Range(0,hei).end; i += ny) y.push_back(i);

    repeat(Mat(x).reshape(1,1), Mat(y).total(), 1, X);
    repeat(Mat(y).reshape(1,1).t(), 1, Mat(x).total(), Y);
}


static void piv(const Mat1f &ma, const Mat1f &mb, Mat1f &dX, Mat1f &dY, const int &wid, const int &hei, const int &t,
                const int &s, const int &gx, const int &gy, const int &th) {
    Mat1i X, Y;
    meshgrid(wid, hei, int(wid/gx), int(hei/gx), X, Y);

    Mat1f cc;
    double cc_center, cc_top, cc_bottom, cc_left, cc_right;
    double cc_min, cc_max;
    Point2i cc_min_p, cc_max_p;

    int i, j;
    float x_sub, y_sub;

    for (int y = 0; y < gy; ++y) {
        for (int x = 0; x < gx; ++x) {
            i = X(y, x) + s + t;
            j = Y(y, x) + s + t;

            // pattern matching
            matchTemplate(
                    ma(Rect(i-t, j-t, 2*t, 2*t)),
                    mb(Rect(i-t-s, j-t-s, 2*(t+s), 2*(t+s))),
                    cc,
                    TM_CCOEFF_NORMED
            );

            // search peak
            minMaxLoc(cc, &cc_min, &cc_max, &cc_min_p, &cc_max_p);

            // sub-pixel interpolation
            if (cc_max * 100 > th) {
                if (cc_max_p.x == 0 || cc_max_p.x == 2 * s || cc_max_p.y == 0 || cc_max_p.y == 2 * s) {
                    x_sub = 0;
                    y_sub = 0;
                } else {
                    cc_center = cc(cc_max_p.y, cc_max_p.x);
                    cc_left = cc(cc_max_p.y, cc_max_p.x-1);
                    cc_right = cc(cc_max_p.y, cc_max_p.x+1);
                    cc_top = cc(cc_max_p.y-1, cc_max_p.x);
                    cc_bottom = cc(cc_max_p.y+1, cc_max_p.x);

                    if (cc_center <= 0 || cc_left <= 0 || cc_right <= 0 || cc_top <= 0 || cc_bottom <= 0) {
                        x_sub = 0;
                        y_sub = 0;
                    } else {
                        x_sub = (log(cc_left) - log(cc_right))
                                / 2 / (log(cc_left) + log(cc_right) - 2 * log(cc_center));
                        y_sub = (log(cc_bottom) - log(cc_top))
                                / 2 / (log(cc_bottom) + log(cc_top) - 2 * log(cc_center));
                    }
                }

                // save dx and dy
                dX(y, x) = cc_max_p.x + x_sub - s;
                dY(y, x) = cc_max_p.y + y_sub - s;
            }
        }
    }
}
