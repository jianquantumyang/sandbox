#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <csignal>

// Флаг для обозначения запроса на завершение программы
volatile sig_atomic_t flag = 0;

// Обработчик сигнала SIGINT (Ctrl+C)
void signalHandler(int signum) {
    flag = 1;
}

int main() {
    // Установка обработчика сигнала
    signal(SIGINT, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGBREAK, signalHandler);
    signal(SIGABRT, signalHandler);

    // Открываем веб-камеру
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Ошибка при открытии веб-камеры." << std::endl;
        return -1;
    }

    // Получаем информацию о видеопотоке
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));

    // Создаем объект VideoWriter для записи видео в файл
    cv::VideoWriter videoWriter("output.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cv::Size(frame_width, frame_height));

    if (!videoWriter.isOpened()) {
        std::cerr << "Ошибка при создании файла для записи видео." << std::endl;
        return -1;
    }

    while (true) {
        cv::Mat frame;
        // Захватываем кадр из видеопотока
        cap >> frame;

        // Проверяем, получен ли кадр
        if (frame.empty()) {
            std::cerr << "Ошибка при захвате кадра." << std::endl;
            break;
        }

        // Записываем кадр в файл
        videoWriter.write(frame);

        // Отображаем кадр
        cv::imshow("Video", frame);

        // Проверяем наличие запроса на завершение программы
        if (flag) {
            break;
        }

        // Проверяем наличие нажатия клавиши 'q' для завершения записи
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Освобождаем ресурсы
    cap.release();
    videoWriter.release();

    // Закрываем окно
    cv::destroyAllWindows();

    return 0;
}
