package lambda_tree;

/**
 * Дерево поиска
 * <p>
 * Все элементы меньше родительского слева от него
 * Все элементы больше родительского справа от него
 */
public class SearchTree<T extends Comparable> {

    Node<T> root = null;

    public void add(T value) {
        Node<T> newNode = new Node<>(value);
        if (root == null) {
            root = newNode;
            return;
        }
        // Ищем куда поставить новый элемент
        Node<T> cur = root;
        while (true) {
            // Мы идём налево если новый элемент меньше текущего
            if (value.compareTo(cur.value) < 0) {
                if (cur.left == null) {
                    cur.left = newNode;
                    return;
                }
                cur = cur.left;
            } else {
                // Иначе (новый элемент => текущему) идём направо
                if (cur.right == null) {
                    cur.right = newNode;
                    return;
                }
                cur = cur.right;
            }
        }
    }

    /**
     * Операция с деревом
     */
    public interface TreeOperation<T> {
        int apply(Node<T> node);
    }

    static class Node<T> {
        T value;
        Node<T> left = null, right = null;

        Node(T value) {
            this.value = value;
        }
    }
}
