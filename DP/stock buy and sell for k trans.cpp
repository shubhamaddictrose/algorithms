// https://www.interviewbit.com/problems/best-time-to-buy-and-sell-stocks-iii/
// video tutorial (Tushar Roy) - https://www.youtube.com/watch?v=oDhu5uGq_ic

/*

https://leetcode.com/problems/best-time-to-buy-and-sell-stock/           
            => find curr min and sell at every pos
https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/        
            => greedy - buy-sell at every peak and valley
https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/       
            => use dp for k=2
https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/
            => make dp table with iterating twice and then optimize keeping maxDiff from 0 to j.

*/
// for infinite transactions use greedy.......

int Solution::maxProfit(const vector<int> &a) {
    int n=a.size();
    if(n==0) return 0;
    int i,j,k=2;
    int dp[k+1][n];
    for(i=0; i<n; i++) dp[0][i] = 0;
    for(i=0; i<k+1; i++) dp[i][0] = 0;
    for(i=1; i<k+1; i++) {
        int md = -a[0];
        for(j=1; j<n; j++) {
            int &r = dp[i][j];
            r = dp[i][j-1];
            r = max(r, a[j]+md);
            md = max(md, dp[i-1][j]-a[j]);
        }
    }
    return dp[2][n-1];
}