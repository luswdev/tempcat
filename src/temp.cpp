#include <string>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <thread>
#include <chrono>

void replace_all(std::string& s, std::string const& toReplace, std::string const& replaceWith)
{
    std::string buf;
    std::size_t pos = 0;
    std::size_t prevPos;

    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos) {
            break;
        }

        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "usage: tempcat <temp_count>\n" << std::endl;
        exit(EINVAL);
    }

    int tempCnt = std::atoi(argv[1]);
    for ( ; ; ) {
        std::string tempStr = "";
        for (int i = 0; i < tempCnt; ++i) {
            std::string cpuTempFile = "/sys/devices/virtual/thermal/thermal_zone" + std::to_string(i) + "/temp";
            std::ifstream ifs(cpuTempFile);
            if (!ifs) {
                std::cout << "open file failed: " << strerror(errno) << std::endl;
                exit(ENOENT);
            }

            std::string buf("");
            std::getline(ifs, buf);
            ifs.close();

            tempStr += buf;
            tempStr += ", ";
        }
        replace_all(tempStr, "\n", "");

        std::cout << "cpu temperature (C): " << tempStr << std::endl;
        std::this_thread::sleep_for(std::chrono::nanoseconds(2 * 1000 * 1000 * 1000));
    }

    return 0;
}
