#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <warehouse.h>>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    Warehouse firstWarehouse;
    Warehouse* warehouseArr[1024];
    Warehouse* currentWarehouse = new Warehouse();
    int CurrentWarehouseId = -1;
    int countWarehouse = 0;

    void sortWarehouse();
    bool isRecord1 = false;             // Флаги для запоминания нажатия
    bool isRecord2 = false;


private slots:
    void fillTable();
    void createWarehouse();
    void saveWarehouse();
    void saveCurrentWarehouse();
    void showBasicAttributes();
    void delWarehouse();
    void currentEdit();
    void setCurrentWarehouse(int row, int column);
};
#endif // MAINWINDOW_H
