
     public  static void main(String[] args) {
        Connection con = null;
        PreparedStatement prepStmt =null;
        ResultSet rs =null;
        int link_id=0;
        for(int i=1;i<45844;i++){
        try{ 
            App t = new App();
            con=t.getConnection();
            String selectSql_1 = "SELECT ID,LINKS FROM weblink WHERE ID = ?;";
            prepStmt = con.prepareStatement(selectSql_1);
            prepStmt.setInt(1,i);
            rs = prepStmt.executeQuery();
            if(rs.next()){
                link_id = rs.getInt(2);
                if(link_id == 0)
                {
                System.out.println("pass one");
                }
            else{
            String id2=String.valueOf(link_id);
            String id1="https://www.themoviedb.org/movie/";
            String id =id1+id2+"/images/backdrops";
            System.out.println(id);
            Document doc = t.getDocument(id);
            Elements elements1 = doc.select(".image_content");
            Element elements2 = elements1.select("a").first();
            String message = elements2.attr("href");
            FileWriter fw = null;
            String file_name = "/my_download/images/"+id2+".txt";
            File f = new File(file_name);
            try {
            if(!f.exists()){
            f.createNewFile();
            }
            fw = new FileWriter(f);
            BufferedWriter out = new BufferedWriter(fw);
            out.write(message, 0, message.length()-1);
            out.close();
            t.closeResultSet(rs);
            t.closePrepStmt(prepStmt);
            t.closeConnection(con);
                 }catch(Exception e){
                        e.printStackTrace();
                 }
       }
         
      }
}

