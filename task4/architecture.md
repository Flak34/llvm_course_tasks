# Описание архитектуры

Используется два типа регистров размера 64 бит:
* **IR** - целочисленные регистры.
* **FR** - вещественные регистры (double).

Функций в модуле всего две - app и compute_julia, каждой из них выделен собственный диапазон регистров:
* **app** - IR1-IR50, FR1-FR50
* **compute_julia** - IR51-IR100, FR51-FR100

В любой аргумент-источник инструкций может подставляться константа вместо регистра:
* целое число (`384`, `-7`) - интерпретируется как int64;
* вещественная дробь (`0.5`, `3.0`) или hex-литерал (`0x3FD14A2340000000`) - 64-битный паттерн double, интерпретируется как double.

Тип операнда-регистра определяется его префиксом (`IR*` - целое, `FR*` - вещественное).

---

## I. Управляющие инструкции

* **`FUNC name(args...)`**  
  Объявляет функцию с именем `name` и перечисляет регистры, из которых будут браться аргументы.
* **`RET`**
  Возврат из текущей функции.
* **`CALL name, args...`**  
  Вызывает функцию `name` с аргументами. I-й аргумент `CALL` передаётся в i-й регистр из объявления `FUNC` соответствующей функции.
* **`JMP label`**  
  Безусловный переход на метку.
* **`JNZ label, IR_cond`**  
  Переход на метку `label`, если целочисленный регистр условия `IR_cond` равен `1`. Если он равен `0`, выполнение 
  продолжается на следующей строке кода.
* **`FLUSH`**  
  simFlush().
* **`PUT_PIXEL IR_x, IR_y, IR_color`**  
  simPutPixel(IR_x, IR_y, IR_color).

---

## II. Управление памятью

* **`ALLOCI IR_dest, size`**  
  Выделяет локальный целочисленный массив размера `size` элементов на стеке. 64-битный адрес начала этого массива записывается в регистр `IR_dest`.

---

## III. Универсальные операции с числами

Данные инструкции универсальны для целых чисел и чисел с плавающей точкой, логика работы и трансляции в LLVM IR
зависит от типов регистров-аргументов.

* **`MOV dest, src` / `MOV dest, constant`**
* **`ADD IR_dest, IR_src1, IR_src2`** / **`ADD FR_dest, FR_src1, FR_src2`**
* **`SUB IR_dest, IR_src1, IR_src2`** / **`SUB FR_dest, FR_src1, FR_src2`**
* **`MUL IR_dest, IR_src1, IR_src2`** / **`MUL FR_dest, FR_src1, FR_src2`**
* **`DIV IR_dest, IR_src1, IR_src2`** / **`DIV FR_dest, FR_src1, FR_src2`**
* **`NEG IR_dest, IR_src`** / **`NEG FR_dest, FR_src`**
* **`SELECT IR_dest, IR_cond, IR_src_true, IR_src_false`** / **`SELECT FR_dest, IR_cond, FR_src_true, FR_src_false`**  
  `dest = (cond == 1) ? src_true : src_false`.
* **`CMP_EQ IR_dest, IR_src1, IR_src2`**, **`CMP_GT IR_dest, IR_src1, IR_src2`**, **`CMP_LT IR_dest, IR_src1, IR_src2`**  
  Сравнивают два числа. Результат всегда записывается в целочисленный регистр `IR_dest`: `1`, если отношение истинно, иначе `0`.

---

## IV. Операции с целыми числами

Данные инструкции применимы только к целым числам.

* **`SREM IR_dest, IR_src1, IR_src2`**
* **`OR IR_dest, IR_src1, IR_src2`**
* **`AND IR_dest, IR_src1, IR_src2`**
* **`SITOFP FR_dest, IR_src`**

---

## V. Кастомные инструкции

Инструкции, выведенные на основе собранной статистики.

* **`MULADD IR_dest, IR_src1, IR_src2, IR_src3`** / **`MULADD FR_dest, FR_src1, FR_src2, FR_src3`**  
  Вычисляет выражение: `(src1 * src2) + src3`.
* **`MULSUB IR_dest, IR_src1, IR_src2, IR_src3`** / **`MULSUB FR_dest, FR_src1, FR_src2, FR_src3`**  
  Вычисляет выражение: `(src1 * src2) - src3`.
* **`MUL2ADD IR_dest, IR_src1, IR_src2, IR_src3`** / **`MUL2ADD FR_dest, FR_src1, FR_src2, FR_src3`**  
  Вычисляет выражение: `(2 * src1 * src2) + src3`.
* **`INC_EQ IR_cond, IR_cnt, IR_limit`**  
  Инкрементирует `IR_cnt` и записывает результат сравнения в `IR_cond`: `IR_cnt++; IR_cond = (IR_cnt == IR_limit)`.
* **`LOAD_SCALE IR_dest, IR_ptr, IR_index, scale_constant`**  
  Загружает значение из памяти по адресу `IR_ptr + IR_index * scale_constant` в `IR_dest`. Нужна для чтения элемента массива по индексу.
* **`STORE_SCALE IR_ptr, IR_index, scale_constant, IR_src`**  
  Записывает `IR_src` в память по адресу `IR_ptr + IR_index * scale_constant`. Нужна для записи элемента массива по индексу.
