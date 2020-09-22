----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04.08.2020 09:13:38
-- Design Name: 
-- Module Name: project_reti_logiche - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity project_reti_logiche is
    Port ( i_clk : in STD_LOGIC;
           i_start : in STD_LOGIC;
           i_rst : in STD_LOGIC;
           i_data : in STD_LOGIC_VECTOR (7 downto 0);
           o_address : out STD_LOGIC_VECTOR (15 downto 0);
           o_done : out STD_LOGIC;
           o_en : out STD_LOGIC;
           o_we : out STD_LOGIC;
           o_data : out STD_LOGIC_VECTOR (7 downto 0));
end project_reti_logiche;

architecture Behavioral of project_reti_logiche is
     type STATE is (START1, READ_WZ,WAIT_WZ, SAVE_WZ, READ_ADDR, WAIT_ADDR, SAVE_ADDR, COMPARE, ENCODE, WRITE_MEM, DONE, CONCLUSION, START2);
     signal present_state: STATE;
     signal addr_ram: std_logic_vector(15 downto 0); 
     signal addr: std_logic_vector(7 downto 0); --valore contenuto in ram(8)
     signal num_wz: std_logic_vector(2 downto 0); --in ENCODE viene usato per codificare la wwz a cui appartiene valore in addr
     signal wz_bit: std_logic; --costante 
     signal uscita: std_logic_vector(7 downto 0) ;  
     signal offset: integer := 0; -- contiene offset che viene calcolato a ogni ciclo     
     signal WZ_1, WZ_2, WZ_3, WZ_4, WZ_5, WZ_6, WZ_7, WZ_8: std_logic_vector(7 downto 0);
     signal WZ_read:  std_logic_vector(15 downto 0);
     
     

begin
    process(i_clk,i_rst)
    begin
        if(i_rst = '1') then 
            present_state <= START1;       
            
            
        elsif(rising_edge(i_clk)) then
            
            case present_state is   
                when START1 =>
                    o_en <= '0';    --riporta a 0 il segnale di lettura a memoria
                    o_we <= '0';    --riporta a 0 il segnale di scrittura a memoria
                    o_done <= '0';  --riporta a 0 il segnale di fine elaborazione   
                    WZ_read <= "0000000000000000";
                    addr <= "00000000";
                    addr_ram <= "0000000000000000";
                    num_wz <= "000";
                    wz_bit <= '0';
                    uscita <= "00000000";
                    offset <= 0;
                    WZ_1 <= "00000000";                  
                    WZ_2 <= "00000000";                  
                    WZ_3 <= "00000000";                  
                    WZ_4 <= "00000000";                  
                    WZ_5 <= "00000000";                  
                    WZ_6 <= "00000000";                  
                    WZ_7 <= "00000000";                  
                    WZ_8 <= "00000000"; 
                                                                                                                                          
                    if (i_start = '1') then                                                      
                        o_en <= '1';
                        o_we <= '0'; 
                        present_state <= READ_WZ;                                                        
                    end if;                                                     
                                    
                when START2 =>
                    o_en <= '0';    --riporta a 0 il segnale di lettura a memoria
                    o_we <= '0';    --riporta a 0 il segnale di scrittura a memoria
                    o_done <= '0';  --riporta a 0 il segnale di fine elaborazione   
                    WZ_read <= "0000000000000000";
                    addr <= "00000000";
                    addr_ram <= "0000000000000000";
                    num_wz <= "000";
                    wz_bit <= '0';
                    uscita <= "00000000";
                    offset <= 0;
                    if (i_start = '1') then                                                      
                        o_en <= '1';
                        o_we <= '0'; 
                        present_state <= READ_ADDR;                                                        
                    end if; 
                                                
                                     
                when READ_WZ =>                                                                                                                         
                    o_address <= WZ_read;        
                    present_state <= WAIT_WZ;
                    
                when WAIT_WZ => 
                    present_state <= SAVE_WZ;
                                    
                when SAVE_WZ =>                                 
                                         if(WZ_read = "0000000000000000") then
                                            WZ_1 <= i_data;                               
                                         elsif(WZ_read = "0000000000000001") then
                                            WZ_2 <= i_data;                               
                                         elsif(WZ_read = "0000000000000010") then
                                            WZ_3 <= i_data;
                                         elsif(WZ_read = "0000000000000011") then
                                            WZ_4 <= i_data;
                                         elsif(WZ_read = "0000000000000100") then
                                             WZ_5 <= i_data;
                                         elsif(WZ_read = "0000000000000101") then
                                             WZ_6 <= i_data;
                                         elsif(WZ_read = "0000000000000110") then
                                             WZ_7 <= i_data;
                                         elsif(WZ_read = "0000000000000111") then
                                             WZ_8 <= i_data;
                                         end if;             
                                         
                                         if( WZ_read = "0000000000000111")then
                                            present_state <= READ_ADDR;
                                         else
                                            WZ_read <=  std_logic_vector(UNSIGNED(WZ_read)+1);
                                            present_state <= READ_WZ; 
                                         end if;                
                    
                when READ_ADDR =>                                                                         
                    o_address <= "0000000000001000";
                    present_state <= WAIT_ADDR;
                                    
                when WAIT_ADDR =>
                    present_state <= SAVE_ADDR;
                                           
                when SAVE_ADDR =>               
                    addr <= i_data;
                    present_state <= COMPARE;                             
                                    when COMPARE =>                     
                                        if((UNSIGNED(addr)-UNSIGNED(WZ_1))<4) then
                                            wz_bit <= '1';
                                            num_wz <= "000";
                                            offset <= to_integer((UNSIGNED(addr)-UNSIGNED(WZ_1)));
                                            present_state <= ENCODE;
                                        elsif((UNSIGNED(addr)-UNSIGNED(WZ_2))<4)then
                                            wz_bit <= '1';
                                            num_wz <= "001";
                                            offset <= to_integer((UNSIGNED(addr)-UNSIGNED(WZ_2)));
                                            present_state <= ENCODE;
                                        elsif((UNSIGNED(addr)-UNSIGNED(WZ_3))<4)then
                                            wz_bit <= '1';
                                            num_wz <= "010";
                                            offset <= to_integer((UNSIGNED(addr)-UNSIGNED(WZ_3)));
                                            present_state <= ENCODE;
                                        elsif((UNSIGNED(addr)-UNSIGNED(WZ_4))<4)then
                                            wz_bit <= '1';
                                            num_wz <= "011";
                                            offset <= to_integer((UNSIGNED(addr)-UNSIGNED(WZ_4)));
                                            present_state <= ENCODE;
                                        elsif((UNSIGNED(addr)-UNSIGNED(WZ_5))<4)then
                                            wz_bit <= '1';
                                            num_wz <= "100";
                                            offset <= to_integer((UNSIGNED(addr)-UNSIGNED(WZ_5)));
                                            present_state <= ENCODE;
                                        elsif((UNSIGNED(addr)-UNSIGNED(WZ_6))<4)then
                                            wz_bit <= '1';
                                            num_wz <= "101";
                                            offset <= to_integer((UNSIGNED(addr)-UNSIGNED(WZ_6)));
                                            present_state <= ENCODE;
                                        elsif((UNSIGNED(addr)-UNSIGNED(WZ_7))<4)then
                                            wz_bit <= '1';
                                            num_wz <= "110";
                                            offset <= to_integer((UNSIGNED(addr)-UNSIGNED(WZ_7)));
                                            present_state <= ENCODE;
                                        elsif((UNSIGNED(addr)-UNSIGNED(WZ_8))<4)then
                                            wz_bit <= '1';
                                            num_wz <= "111";
                                            offset <= to_integer((UNSIGNED(addr)-UNSIGNED(WZ_8)));
                                            present_state <= ENCODE;
                                        else 
                                            wz_bit <= '0';
                                            uscita(7) <= wz_bit;
                                            uscita(6 downto 0) <= addr(6 downto 0);
                                            present_state <= WRITE_MEM;            
                                        
                                        end if;                       
                                            
                when ENCODE =>                
                    uscita(7) <= wz_bit;
                    uscita(6 downto 4) <= num_wz;                                 
                    if(offset=0) then                
                        uscita(3 downto 0) <= "0001";
                    elsif(offset=1) then
                        uscita(3 downto 0) <= "0010";
                    elsif(offset=2) then
                        uscita(3 downto 0) <= "0100";
                    elsif(offset=3) then
                        uscita(3 downto 0) <= "1000";                        
                    end if;                        
                    present_state <= WRITE_MEM;
                                        
                when WRITE_MEM =>                 
                    o_en <= '1';
                    o_we <= '1';
                    o_address <= "0000000000001001";
                    o_data <= uscita;                    
                    present_state <= DONE;
                                   
                                      
                when DONE =>                
                    o_we <= '0';
                    o_done <= '1';
                    o_en <= '0';
                    present_state <= CONCLUSION;                                 
                                            
                when CONCLUSION =>                        
                    if (i_start = '0') then                 
                        o_done <= '0';
                        present_state <= START2;
                    end if;
                                                              
            end case;                                                                
        end if;
    end process;                    
end Behavioral;