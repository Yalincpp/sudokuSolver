#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>

auto sudokuGirisi() -> std::vector<std::vector<int>>;
auto girilenSudokuGecerliMi(std::vector<std::vector<int>> sudokuTablosu) -> bool;
auto sudokuCozulduMu(std::vector<std::vector<int>> sudokuTahtasi) -> bool;
auto sudokuyuCoz(std::vector<std::vector<int>>& sudokuTahtasi) -> bool;
void tahtayiYazdir(std::vector<std::vector<int>> tahta);
auto sifirlarlaDoldur() -> std::vector<std::vector<int>>;

int main()
{
    std::string enter;

    std::vector<std::vector<int>> sudokuTablosu = sudokuGirisi();
    if (!girilenSudokuGecerliMi(sudokuTablosu))
    {
        std::cout << "Hata 1: Hatali Sudoku Bulmacasi. Girdiginiz bulmaca bir veya daha cok hata iceriyor. Lutfen tekrar deneyin. Girdiginiz bulmaca: " << std::endl;
        tahtayiYazdir(sudokuTablosu);
        return 1;
    }

    if (sudokuyuCoz(sudokuTablosu))
    {
        if (!girilenSudokuGecerliMi(sudokuTablosu))
        {
            std::cout << "Hata 3: Hatali sudoku cozumu. Sudoku icin bir cozume ulasildi ancak bu cozumun dogru olmadigi tespit edildi. Kontrol icin son durum yazdiriliyor:" << std::endl;
            tahtayiYazdir(sudokuTablosu);
            return 3;
        }
        std::cout << "Sudoku cozumu basarili! Cozulmus hal: " << std::endl;
        tahtayiYazdir(sudokuTablosu);
        std::cout << "Devam etmek icin ENTER tusuna basiniz . . . ";
        std::cin.ignore(100, '\n');
        std::cin.get();
        return 0;
    }

    std::cout << "Hata 2: Sudoku cozumu basarisiz oldu. Sudokuyu gozden gecirmek isteyebilirsiniz." << std::endl;
    return 2;
}

auto sudokuGirisi() -> std::vector<std::vector<int>>
{
    int girilenSatirSayisi = 0;
    int satirElemani;
    int secim;

    std::vector<std::vector<int>> sudokuTablosu;

    std::cout << "Manuel giris icin 1, dosyadan (sudoku.txt) otomatik cekmek icin 2 giriniz: ";
    std::cin >> secim;

    if (secim == 1)
    {
        std::cout << "Sudoku girisi yapiniz: " << std::endl;
        for (int i = 0; i < 9; ++i)
        {
            for (int j = 0; j < 9; ++j)
            {
                if (j == 0)
                {
                    sudokuTablosu.push_back(std::vector<int>());
                }
                std::cout << "Lutfen " << i + 1 << ". satir, " << j + 1 << ". sutundaki rakami giriniz. Bilinmiyorsa 0 giriniz:" << std::endl;
                std::cin >> satirElemani;
                sudokuTablosu.at(i).push_back(satirElemani);
            }
        }
        std::cout << "Sudoku girisi tamamlandi. Cozume geciliyor..." << std::endl;
    }

    else if (secim == 2)
    {
        auto sudokuDosyasi = std::ifstream{"sudoku.txt"};

        char hucre;
        int sayac = 0;

        sudokuTablosu = sifirlarlaDoldur();

        while (sudokuDosyasi.get(hucre)) {
            if (hucre != ' ' && hucre != '\n') {
                int satir = sayac / 9;
                int sutun = sayac % 9;
                sudokuTablosu.at(satir).at(sutun) = hucre - '0';
                sayac++;
            }
        }

        tahtayiYazdir(sudokuTablosu);
    }

    return sudokuTablosu;
}

auto girilenSudokuGecerliMi(std::vector<std::vector<int>> sudokuTablosu) -> bool
{
    for (int i = 0; i < 9; ++i) // Satir kontrol
    {
        for (int j = 1; j <= 9; ++j)
        {
            if (std::count(sudokuTablosu.at(i).begin(), sudokuTablosu.at(i).end(), j) > 1)
            {
                return false;
            }
        }
    }

    for (int i = 0; i < 9; ++i) // Sutun kontrol
    {
        std::vector<int> sutun;
        for (int j = 0; j < 9; ++j)
        {
            sutun.push_back(sudokuTablosu.at(j).at(i));
        }

        for (int j = 0; j < 9; ++j)
        {
            if (std::count(sutun.begin(), sutun.end(), (j + 1)) > 1)
            {
                return false;
            }
        }
    }

    for (int i = 0; i < 9; i+=3) // 3x3 blok kontrol
    {
        for (int j = 0; j < 9; j+=3)
        {
            std::vector<int> blok;
            for (int k = i; k < i + 3; ++k)
            {
                for (int l = j; l < j + 3; ++l)
                {
                    blok.push_back(sudokuTablosu.at(k).at(l));
                }
            }
            for (int k = 0; k < 9; ++k)
            if (std::count(blok.begin(), blok.end(), k + 1) > 1)
            {
                return false;
            }
        }
    }

    return true;
}

auto sudokuCozulduMu(std::vector<std::vector<int>> sudokuTahtasi) -> bool
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (sudokuTahtasi.at(i).at(j) == 0)
            {
                return false;
            }
        }
    }

    return true;
}

auto sudokuyuCoz(std::vector<std::vector<int>>& sudokuTahtasi) -> bool
{
    if (sudokuCozulduMu(sudokuTahtasi) || !girilenSudokuGecerliMi(sudokuTahtasi))
    {
        return true;
    }

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (sudokuTahtasi.at(i).at(j) == 0)
            {
                for (int k = 1; k < 10; ++k)
                {
                    sudokuTahtasi.at(i).at(j) = k;
                    if (girilenSudokuGecerliMi(sudokuTahtasi))
                    {
                        if (sudokuyuCoz(sudokuTahtasi))
                        {
                            return true;
                        }
                    }
                    sudokuTahtasi.at(i).at(j) = 0;
                }
                return false;
            }
        }
    }

    return true;
}

void tahtayiYazdir(std::vector<std::vector<int>> tahta)
{
    for (int i = 0; i < 9; ++i)
    {
        if (i % 3 == 0)
        {
            std::cout << "-------------------------" << std::endl;
        }

        for (int j = 0; j < 9; ++j)
        {
            if (j % 3 == 0)
            {
                std::cout << "| ";
            }
            std::cout << tahta.at(i).at(j) << " ";
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}

auto sifirlarlaDoldur() -> std::vector<std::vector<int>>
{
    return 
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
}
