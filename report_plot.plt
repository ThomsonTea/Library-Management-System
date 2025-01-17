set terminal png size 1024,768
set output 'annual_report_2025.png'
set title 'Annual Report - 2025 (Generated on: 2025-01-18 05:02:40)'
set xlabel 'Month'
set ylabel 'Count'
set grid
set key outside
set xtics 1
plot 'report_data.txt' using 1:2 with lines title 'Returned', \
     'report_data.txt' using 1:3 with lines title 'Damaged', \
     'report_data.txt' using 1:4 with lines title 'Lost', \
     'report_data.txt' using 1:5 with lines title 'Total'
