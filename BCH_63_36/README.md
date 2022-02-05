## Название: Neural min-sum ofset flooding decoder

Зависимости: пока нет

Функциональное назначение: улучшить алгоритм из 
https://arxiv.org/pdf/1701.05931.pdf
https://github.com/lorenlugosch/neural-min-sum-decoding
Логическая структура: Статья в папке Documentation исходники в папке BCH_63_36

### Компиляция:
Выполнить все предписания из XRT_compile_run_profiling.md
```bash
g++ -g2 ndpc.cpp alist.c queue.c interleaver.c wgn.c
```


### Запуск 

Запустить исполняемый файл ввести Eb_N0.
Результат значения BER, FER для флудящего алгоритма с минимальным обучением.
BER1 FER1 - характеристики имитируемого жесткого декодера