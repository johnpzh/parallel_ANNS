
for num_t in 2 4 8; do
    cat output.PSS_T${num_t}_P0.9*.table.txt > table_T${num_t}.txt
    python3 ../../scripts/output_find_runtime_above_presicion.py table_T${num_t}.txt selected_T${num_t}.txt 0 2
done