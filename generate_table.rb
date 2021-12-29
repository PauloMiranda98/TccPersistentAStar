def get_time(line)
  aux = line.split(' ')
  if aux.size >= 3
    aux[2].to_f
  else
    nil
  end
end

def format_array(a)
  if a.size == 0
    "?"
  else
    "#{(a.sum(0.0)/a.size).round(3)} \\# #{a.size}"
  end
end

i = 0
file = File.open("output_test.txt")
lines = file.readlines

while i < lines.size do
  file_name = lines[i].split('/')[2]
  aux = file_name.split('_')
  size = aux[0]
  it = aux[1]
  
  
  ida_star = []
  classic_a_star = []
  ida_star_with_memorization = []
  persistent_a_star = []
  
  for t in 1..5 do
    ida_star_line = lines[i+1]
    t = get_time(ida_star_line)
    ida_star.append(t) if t
    
    classic_a_star_line = lines[i+2]
    t = get_time(classic_a_star_line)
    classic_a_star.append(t) if t

    ida_star_with_memorization_line = lines[i+3]
    t = get_time(ida_star_with_memorization_line)
    ida_star_with_memorization.append(t) if t

    persistent_a_star_line = lines[i+4]
    t = get_time(persistent_a_star_line)
    persistent_a_star.append(t) if t

    i += 6
  end

  line  = "#{size} & #{it}"
  line += " & #{format_array(ida_star)}"
  line += " & #{format_array(ida_star_with_memorization)}"
  line += " & #{format_array(classic_a_star)}"
  line += " & #{format_array(persistent_a_star)}"  
  line += " \\\\"
  
  puts(line)
end
