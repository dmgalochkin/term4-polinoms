#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QLabel>

#include "TPolinomManager.hpp"

class PolynomialWidget;
class TableWidget;
class ExpressionWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onNewPolynomial();
  void onExit();
  void onAbout();
  void onShowHelp();
  void onTableChanged();
  void onPolynomialChanged();

private:
  void setupUI();
  void setupMenuBar();
  void setupStatusBar();
  void setupConnections();
  void updateStatusBar();
  void updateWindowTitle();

  QTabWidget *m_tabWidget;
  PolynomialWidget *m_polynomialWidget;
  TableWidget *m_tableWidget;
  ExpressionWidget *m_expressionWidget;
  QLabel *m_statusLabel;
  QLabel *m_tableCountLabel;
  QLabel *m_activeTableLabel;
  QAction *m_newAction;
  QAction *m_exitAction;
  QAction *m_aboutAction;
  QAction *m_helpAction;
  TPolinomManager* m_polynomManager;

protected:
  void closeEvent(QCloseEvent *event) override;
};

#endif