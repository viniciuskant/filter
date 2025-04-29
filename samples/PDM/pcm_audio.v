module PCM_Audio #(
    parameter AUDIO_FILE = "dump.hex"
) (
    input wire clk,
    input wire rst_n,

    input wire pcm_read_i,
    output reg pcm_ready_o,
    output reg [15:0] pcm_data_o
);

    reg [31:0] pointer;
    reg [15:0] memory [0:262143];

    initial begin
        if (AUDIO_FILE != "") begin
            $readmemh(AUDIO_FILE, memory);
        end
    end

    always @(posedge clk or negedge rst_n) begin
        pcm_ready_o <= 1'b0;

        if(rst_n) begin
            pointer <= 'd0;
        end else begin
            if(pcm_read_i) begin
                pcm_ready_o <= 1'b1;
                pcm_data_o  <= memory[pointer];
                pointer     <= pointer + 1;
            end
        end
    end

endmodule

