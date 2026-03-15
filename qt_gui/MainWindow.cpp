#include "MainWindow.h"
#include "PolynomialWidget.h"
#include "TableWidget.h"
#include "ExpressionWidget.h"

#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , m_tabWidget(nullptr)
  , m_polynomialWidget(nullptr)
  , m_tableWidget(nullptr)
  , m_expressionWidget(nullptr)
  , m_statusLabel(nullptr)
  , m_tableCountLabel(nullptr)
  , m_activeTableLabel(nullptr)
  , m_polynomManager(new TPolinomManager())
{
  setupUI();
  setupMenuBar();
  setupStatusBar();
  setupConnections();
  updateStatusBar();
  updateWindowTitle();
}

MainWindow::~MainWindow()
{
  delete m_polynomManager;
}

void MainWindow::setupUI()
{
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  m_tabWidget = new QTabWidget();
  m_polynomialWidget = new PolynomialWidget(m_polynomManager);
  m_tabWidget->addTab(m_polynomialWidget, "Полиномы");
  m_tableWidget = new TableWidget(m_polynomManager);
  m_tabWidget->addTab(m_tableWidget, "Таблицы");
  m_expressionWidget = new ExpressionWidget(m_polynomManager);
  m_tabWidget->addTab(m_expressionWidget, "Выражения");
  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->addWidget(m_tabWidget);
  setWindowTitle("Система алгебры полиномов");
  setMinimumSize(800, 600);
  resize(1000, 700);
}

void MainWindow::setupMenuBar()
{
  QMenu *fileMenu = menuBar()->addMenu("&Файл");
  m_newAction = new QAction("&Новый полином", this);
  m_newAction->setShortcut(QKeySequence::New);
  m_newAction->setStatusTip("Создать новый полином");
  fileMenu->addAction(m_newAction);
  fileMenu->addSeparator();
  m_exitAction = new QAction("В&ыход", this);
  m_exitAction->setShortcut(QKeySequence::Quit);
  m_exitAction->setStatusTip("Выйти из приложения");
  fileMenu->addAction(m_exitAction);
  QMenu *helpMenu = menuBar()->addMenu("&Справка");
  m_helpAction = new QAction("&Помощь", this);
  m_helpAction->setShortcut(QKeySequence::HelpContents);
  m_helpAction->setStatusTip("Показать справку");
  helpMenu->addAction(m_helpAction);
  helpMenu->addSeparator();
  m_aboutAction = new QAction("&О программе", this);
  m_aboutAction->setStatusTip("Информация о программе");
  helpMenu->addAction(m_aboutAction);
}

void MainWindow::setupStatusBar()
{
  m_statusLabel = new QLabel("Готов");
  statusBar()->addWidget(m_statusLabel);
  statusBar()->addPermanentWidget(new QLabel(" | "));
  m_activeTableLabel = new QLabel("Активная таблица: Array");
  statusBar()->addPermanentWidget(m_activeTableLabel);
  statusBar()->addPermanentWidget(new QLabel(" | "));
  m_tableCountLabel = new QLabel("Полиномов: 0");
  statusBar()->addPermanentWidget(m_tableCountLabel);
}

void MainWindow::setupConnections()
{
  connect(m_newAction, &QAction::triggered, this, &MainWindow::onNewPolynomial);
  connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);
  connect(m_aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
  connect(m_helpAction, &QAction::triggered, this, &MainWindow::onShowHelp);
  connect(m_tableWidget, &TableWidget::tableChanged, this, &MainWindow::onTableChanged);
  connect(m_polynomialWidget, &PolynomialWidget::polynomialChanged, this, &MainWindow::onPolynomialChanged);
}

void MainWindow::updateStatusBar()
{
  if (m_polynomManager) {
    auto* activeTable = m_polynomManager->GetActiveTable();
    size_t count = activeTable->GetSize();
    m_tableCountLabel->setText(QString("Полиномов: %1").arg(count));
    QString tableName = "Array Table";
    m_activeTableLabel->setText(QString("Активная таблица: %1").arg(tableName));
  }
}

void MainWindow::updateWindowTitle()
{
  setWindowTitle("Система алгебры полиномов");
}

void MainWindow::onNewPolynomial()
{
  m_tabWidget->setCurrentWidget(m_polynomialWidget);
  m_polynomialWidget->createNewPolynomial();
}

void MainWindow::onExit()
{
  close();
}

void MainWindow::onAbout()
{
  QMessageBox::about(this, "О программе",
    "<h3>Система алгебры полиномов</h3>"
    "<p>Версия 1.0</p>"
    "<p>Лабораторная работа по структурам данных</p>"
    "<p>ННГТУ им. Р.Е. Алексеева</p>"
    "<p><b>Возможности:</b></p>"
    "<ul>"
    "<li>Работа с полиномами от 3 переменных</li>"
    "<li>4 типа таблиц для хранения</li>"
    "<li>Арифметические операции</li>"
    "<li>Дифференцирование и интегрирование</li>"
    "<li>Вычисление постфиксных выражений</li>"
    "</ul>");
}

void MainWindow::onShowHelp()
{
  QMessageBox::information(this, "Справка",
    "<h3>Как использовать программу:</h3>"
    "<p><b>Вкладка \"Полиномы\":</b></p>"
    "<ul>"
    "<li>Создание и редактирование полиномов</li>"
    "<li>Просмотр существующих полиномов</li>"
    "<li>Операции дифференцирования и интегрирования</li>"
    "</ul>"
    "<p><b>Вкладка \"Таблицы\":</b></p>"
    "<ul>"
    "<li>Переключение между типами таблиц</li>"
    "<li>Просмотр содержимого таблиц</li>"
    "<li>Управление полиномами в таблицах</li>"
    "</ul>"
    "<p><b>Вкладка \"Выражения\":</b></p>"
    "<ul>"
    "<li>Вычисление арифметических выражений</li>"
    "<li>Использование имен полиномов в выражениях</li>"
    "<li>Сохранение результатов вычислений</li>"
    "</ul>");
}

void MainWindow::onTableChanged()
{
  updateStatusBar();
}

void MainWindow::onPolynomialChanged()
{
  updateStatusBar();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  event->accept();
}
