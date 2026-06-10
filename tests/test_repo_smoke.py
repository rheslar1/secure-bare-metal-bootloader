import pathlib
import unittest


class RepoSmokeTest(unittest.TestCase):
    def setUp(self):
        self.root = pathlib.Path(__file__).resolve().parents[1]

    def test_required_artifacts_exist(self):
        required = [
            'README.md',
            'ARCHITECTURE.md',
            'CMakeLists.txt',
            'src/main.c',
            'docs/validation-plan.md',
            '.github/workflows/ci.yml',
        ]
        for relative_path in required:
            with self.subTest(relative_path=relative_path):
                self.assertTrue((self.root / relative_path).exists())

    def test_readme_names_project(self):
        readme = (self.root / 'README.md').read_text(encoding='utf-8')
        self.assertIn("Secure Bare-Metal Bootloader", readme)


if __name__ == '__main__':
    unittest.main()
