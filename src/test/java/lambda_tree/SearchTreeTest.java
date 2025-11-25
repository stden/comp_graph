package lambda_tree;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

/**
 * Операции над деревом с лябда-выражениями
 * Пример рекурсивного лямбда-выражения
 */
public class SearchTreeTest extends Assert {
    SearchTree.TreeOperation<Integer> depth;
    SearchTree<Integer> tree;

    @Before
    public void setUp() {
        tree = new SearchTree<>();
        tree.add(2);
        tree.add(1);
        tree.add(5);
        //    2
        //  /   \
        // 1    5
    }

    @Test
    public void testTreeLambda() {
        depth = node -> {
            if (node == null)
                return 0;
            return Math.max(depth.apply(node.left), depth.apply(node.right))
                    + 1;
        };
        assertEquals(2, depth.apply(tree.root));
    }
}
