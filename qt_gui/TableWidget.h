#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QSplitter>
#include <QHeaderView>

#include "TPolinomManager.hpp"

class TableWidget : public QWidget
{
  Q_OBJECT

public:
  explicit TableWidget(TPolinomManager* manager, QWidget *parent = nullptr);

public slots:
  void onTableTypeChanged();
  void onSearchTextChanged();
  void onPolynomialSelected();
  void refreshTableView();
  void removeSelectedPolynomial();
  void clearAllPolynomials();

signals:
  void tableChanged();

private:
  void setupUI();
  void setupConnections();
  void updateTableView();
  void populateTableWidget();
  QString getTableTypeName(int index);

  QSplitter *m_mainSplitter;
  QComboBox *m_tableTypeCombo;
  QLineEdit *m_searchEdit;
  QPushButton *m_refreshButton;
  QPushButton *m_clearButton;
  QTableWidget *m_tableWidget;
  QPushButton *m_removeButton;
  TPolinomManager* m_manager;
  int m_currentTableType;
};

#endif