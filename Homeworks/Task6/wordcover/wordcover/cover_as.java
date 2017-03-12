import java.util.*;
import java.math.*;
import java.io.*;

class cover_as implements Runnable {
    public void solve() throws IOException {
        Scanner in = new Scanner(new File("cover.in"));
        PrintWriter out = new PrintWriter(new File("cover.out"));

        char[] c = in.next().toCharArray();
        int n = c.length;
        assert 1 <= n && n <= 250000;

        int[] b = new int[n];
        int[] q = new int[n];
        int[] r = new int[n];

        int k = 0;
        q[0] = 0;
        r[0] = 0;
        for (int i = 1; i < n; i++) {
            if (i == n - 1) {
                System.out.println("");
            }
            while (k > 0 && c[i] != c[k]) {
                k = b[k - 1];
            }
            if (c[i] == c[k]) {
                k++;
            }
            b[i] = k;

            if (k != 0 && r[q[k - 1]] + q[k - 1] + 1 >= i) {
                q[i] = q[k - 1];
                r[q[i]] = i;
            } else {
                q[i] = i;
                r[i] = i;
            }
        }

        System.err.println(Arrays.toString(b));
        System.err.println(Arrays.toString(q));
        System.err.println(Arrays.toString(r));
        for (int i = 0; i < n; i++) {
            out.print((q[i] + 1) + " ");
        }

        in.close();
        out.close();
    }

    public void run() {
        try {
            solve();
        } catch (IOException e) {
            e.printStackTrace();
            throw new AssertionError();
        }
    }

    public static void main(String[] args) {
        new Thread(new cover_as()).start();
    }
}