#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Первым делом необходимо создать объект, который будет использоваться для работы с данными нашей БД
     * и инициализировать подключение к базе данных
     * */
    db = new DataBase();
    db->connectToDataBase();

    /* После чего производим наполнение таблицы базы данных
     * контентом, который будет отображаться в TableView
     * */
    list_name = {"Дмитрий", "Александр", "Василий", "Иглы", "Петр", "Анна", "София", "Елизавета", "Вячеслав", "Роман", "Альберт", "Никита", "Юрий", "Ульян", "Наталья", "Иван", "Михаил", "Артем", "Кирил"};
    list_init = {"А.", "Б.", "В.", "Г.", "Д.", "Е.", "Ж.", "З.", "И.", "К.", "Л.", "М.", "Н.", "О.", "П.", "Р.", "С.", "Т.", "У.", "Ф.", "Х.", "Ы.", "Ю.", "Э.", "Я."};

    connect(ui->butt_add, SIGNAL(clicked()), this, SLOT(slot_butt_add()));
    connect(ui->butt_del, SIGNAL(clicked()), this, SLOT(slot_butt_del()));

    /* Инициализируем модель для представления данных
     * с заданием названий колонок
     * */
    this->setupModel(TABLE,
                     QStringList() << "id"
                     << "Имя"
                     << "Возрост"
                     << "Рост"
                     << "Вес"
                     );
    /* Инициализируем внешний вид таблицы с данными
     * */
    this->createUI();
}

void MainWindow::slot_butt_add(){
    QVariantList data;
    data.append(list_name[Rand(0, list_name.size() - 1)] + " "
            + list_init[Rand(0, list_init.size() - 1)]
            + list_init[Rand(0, list_init.size() - 1)]);
    data.append(Rand(17, 24));
    data.append(Rand(150, 200));
    data.append(Rand(60, 120));
    // Вставляем данные в БД
    db->inserIntoTable(data);
    model->select();
}

void MainWindow::slot_butt_del(){
    if(ui->tableView->currentIndex().column() != -1){
        db->deleteFromTable(model->record(ui->tableView->currentIndex().row()).value("id").toString());
        model->select();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Метод для инициализации модели представления данных
 * */
void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
    model = new QSqlTableModel(this);
    model->setTable(tableName);

    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0,Qt::AscendingOrder);
}

void MainWindow::createUI()
{
    ui->tableView->setModel(model);     // Устанавливаем модель на TableView
    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одной строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок
//    ui->tableView->setColumnWidth(0, 40);
    ui->tableView->setColumnWidth(1, 130);
    ui->tableView->setColumnWidth(2, 80);
    ui->tableView->setColumnWidth(3, 40);
    ui->tableView->setColumnWidth(4, 40);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    // Реализуем скролл пальцем
    QScroller::grabGesture(ui->tableView->viewport(), QScroller::TouchGesture);
    ui->tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableView->grabGesture(Qt::SwipeGesture);
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Разрешить сортировку таблицы
    ui->tableView->setSortingEnabled(true);

    model->select(); // Делаем выборку данных из таблицы
}

int Rand(int min, int max){
    if (min == max){
        return min;
    }
    std::uniform_int_distribution dist(min, max);
    return dist(*QRandomGenerator::global());
}
