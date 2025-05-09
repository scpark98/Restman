[개발 배경]
- Postman처럼 API를 테스트하는 기능은 유사하지만
  반복 요청, thread로 요청 등 원하는 테스트 방식을 지원하기 위해 제작함.

[UI 구성]
- left tree에는 등록된 API treeview
- right는 단일 API 테스트일 경우는 Postman과 동일하나
  스위칭하면 여러 API들을 병렬적으로 테스트 할 수 있는 listview로 표시

[저장 포맷]

{
	"collection":"test",
	"folder":"test folder",
	"name":"testAPI",
	"method":"GET",
	"url":"http://admin.linkmemine.com:443/lmm/api/v1.0/"
}

- json, 실행파일 아래 data 폴더 아래 collection/folder/name.json으로 load/save된다.
  즉, data/test/test folder/testAPI.json 파일로 저장된다.

[load]
- data 폴더내의 모든 json 파일을 검색한 후 순차적으로 파싱하여 로딩.
- 