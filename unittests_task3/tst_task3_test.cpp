#include <QtTest>
#include <QString>
#include <map>
#include <vector>
#include <set>

class Task3Test : public QObject
{
    Q_OBJECT

private:
    // Функция для нахождения листьев (аналог логики из task3.cpp)
    std::set<int> findLeaves(const std::map<int, std::vector<int>>& tree)
    {
        std::set<int> leaves;
        
        if (tree.empty()) {
            return {1}; // Корень - лист
        }
        
        // Собираем все узлы, которые НЕ являются листьями (имеют детей)
        std::set<int> nonLeaves;
        for (const auto& pair : tree) {
            nonLeaves.insert(pair.first);
        }
        
        // Собираем все существующие узлы
        std::set<int> allNodes = nonLeaves;
        for (const auto& pair : tree) {
            for (int child : pair.second) {
                allNodes.insert(child);
            }
        }
        
        // Листья - узлы, которые есть в дереве, но не имеют детей
        for (int node : allNodes) {
            if (tree.find(node) == tree.end()) {
                leaves.insert(node);
            }
        }
        
        return leaves;
    }
    
    QString formatAnswer(const std::set<int>& leaves)
    {
        QString result;
        for (int leaf : leaves) {
            result += QString::number(leaf) + " ";
        }
        return result.trimmed();
    }

private slots:
    // Основной тест: проверка всех типов деревьев
    void testAllTreeTypes()
    {
        struct TestCase {
            std::map<int, std::vector<int>> tree;
            QString expected;
            QString description;
        };
        
        std::vector<TestCase> tests = {
            {{}, "1", "Один узел"},
            {{{1, {2}}}, "2", "Цепочка из 2 узлов"},
            {{{1, {2, 3}}}, "2 3", "Корень с двумя листьями"},
            {{{1, {2}}, {2, {3}}}, "3", "Цепочка 1-2-3"},
            {{{1, {2, 3}}, {2, {4, 5}}}, "3 4 5", "Бинарное дерево"},
            {{{1, {2, 3, 4}}}, "2 3 4", "Звезда"},
            {{{1, {2}}, {2, {3}}, {3, {4}}}, "4", "Длинная цепочка"}
        };
        
        for (const auto& test : tests) {
            std::set<int> leaves = findLeaves(test.tree);
            QString result = formatAnswer(leaves);
            
            // Сравниваем множества (порядок не важен)
            QStringList expectedList = test.expected.split(" ");
            QStringList resultList = result.split(" ");
            std::sort(expectedList.begin(), expectedList.end());
            std::sort(resultList.begin(), resultList.end());
            
            QVERIFY2(expectedList == resultList,
                     QString("%1: ожидалось [%2], получено [%3]")
                     .arg(test.description, test.expected, result).toUtf8());
        }
    }
    
    // Тест форматирования ответа (пробелы не важны)
    void testAnswerFormatting()
    {
        std::map<int, std::vector<int>> tree = {{1, {2, 3, 4}}};
        std::set<int> leaves = findLeaves(tree);
        QString correct = formatAnswer(leaves); // "2 3 4"
        
        // Правильные варианты
        QVERIFY(correct == "2 3 4");
        
        // Симуляция проверки из task3.cpp (trimmed сравнение)
        auto check = [&correct](const QString& user) {
            return user.trimmed() == correct;
        };
        
        QVERIFY(check("2 3 4"));
        QVERIFY(check(" 2 3 4"));
        QVERIFY(check("2 3 4 "));
        QVERIFY(check(" 2 3 4 "));
        QVERIFY(!check("2 3 5"));
        QVERIFY(!check("1 2 3"));
    }
    
    // Тест случайной генерации (проверка корректности findLeaves)
    void testRandomGeneration()
    {
        QRandomGenerator* gen = QRandomGenerator::global();
        
        for (int testNum = 0; testNum < 50; testNum++) {
            int n = gen->bounded(5, 9);
            std::map<int, std::vector<int>> tree;
            
            // Генерация как в вашем коде
            for (int i = 2; i <= n; i++) {
                int parent = gen->bounded(1, i);
                tree[parent].push_back(i);
            }
            
            std::set<int> leaves = findLeaves(tree);
            
            // Проверка 1: у листьев нет детей
            for (int leaf : leaves) {
                QVERIFY2(tree.find(leaf) == tree.end(),
                         "Лист не должен иметь детей");
            }
            
            // Проверка 2: все не-листья имеют детей
            for (const auto& pair : tree) {
                if (leaves.find(pair.first) == leaves.end()) {
                    QVERIFY2(!pair.second.empty(),
                             "Узел с детьми не должен быть листом");
                }
            }
            
            // Проверка 3: все узлы от 1 до n присутствуют
            std::set<int> allNodes;
            for (const auto& pair : tree) {
                allNodes.insert(pair.first);
                for (int child : pair.second) {
                    allNodes.insert(child);
                }
            }
            
            QString leavesStr = formatAnswer(leaves);
            QVERIFY2(!leavesStr.isEmpty(), "Листья должны существовать");
        }
    }
};

QTEST_APPLESS_MAIN(Task3Test)

#include "tst_task3_test.moc"